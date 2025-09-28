#include "client/core/client_core.h"

#include "chat/chat.h"
#include "chat_system/chat_system.h"
#include "dto/dto_struct.h"
#include "exceptions_cpp/login_exception.h"
#include "exceptions_qt/exception_login.h"
#include "exceptions_qt/exception_network.h"
#include "exceptions_qt/exception_router.h"
#include "message/message.h"
#include "message/message_content_struct.h"
#include "system/date_time_utils.h"
#include "system/picosha2.h"
#include "system/serialize.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"

#include <QString>

#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <netinet/in.h>

namespace {
constexpr int kInvalidSocket = -1;
}

ClientCore::ClientCore(ChatSystem &chat_system, QObject *parent)
    : QObject(parent), chat_system_(chat_system), transport_(), request_dispatcher_(chat_system_, transport_),
      dto_writer_(*this), dto_builder_(*this), create_objects_(*this), modify_objects_(*this) {}

ClientCore::~ClientCore() = default;

bool ClientCore::getIsServerOnlineCore() const noexcept { return status_online_.load(std::memory_order_acquire); }

ServerConnectionConfig &ClientCore::getServerConnectionConfigCore() { return server_connection_config_; }

const ServerConnectionConfig &ClientCore::getServerConnectionConfigCore() const { return server_connection_config_; }

ServerConnectionMode &ClientCore::getServerConnectionModeCore() { return server_connection_mode_; }

const ServerConnectionMode &ClientCore::getServerConnectionModeCore() const { return server_connection_mode_; }

ChatSystem &ClientCore::getInstance() { return chat_system_; }

const std::shared_ptr<User> ClientCore::getActiveUserCl() const { return chat_system_.getActiveUser(); }

int ClientCore::getSocketFdCore() const {
  std::scoped_lock lock(socket_mutex_);
  return socket_fd_;
}

void ClientCore::setActiveUserCl(const std::shared_ptr<User> &user) { chat_system_.setActiveUser(user); }

void ClientCore::setSocketFdCore(int socket_fd) {
  {
    std::scoped_lock lock(socket_mutex_);
    socket_fd_ = socket_fd;
  }

  if (socket_fd_ < 0) {
    updateConnectionState(false, ServerConnectionMode::Offline);
  } else {
    updateConnectionState(true, server_connection_mode_);
  }
}

std::optional<std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>>> ClientCore::getChatListCl() {
  const auto &active_user = getActiveUserCl();
  if (!active_user) {
    return std::nullopt;
  }

  const auto &chat_list = active_user->getUserChatList();
  if (!chat_list) {
    return std::nullopt;
  }

  auto chats = chat_list->getChatFromList();

  std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>> result;
  result.clear();

  if (chats.empty()) {
    return result;
  }

  for (const auto &chat : chats) {
    auto chat_ptr = chat.lock();
    if (!chat_ptr) {
      continue;
    }

    auto chat_dto = fillChatDTOCl(chat_ptr);
    if (!chat_dto.has_value()) {
      continue;
    }

    const auto &messages = chat_ptr->getMessages();
    std::int64_t last_message_time_stamp = 0;

    if (!messages.empty()) {
      last_message_time_stamp = messages.rbegin()->first;
    }

    result.insert({last_message_time_stamp, chat_dto.value()});
  }

  return result;
}

const std::vector<UserDTO> ClientCore::findUserByTextPartOnServerCl(const std::string &text_to_find) {
  UserLoginPasswordDTO user_login_password_dto;
  user_login_password_dto.login = chat_system_.getActiveUser()->getLogin();
  user_login_password_dto.passwordhash = text_to_find;

  PacketDTO packet_dto;
  packet_dto.requestType = RequestType::RqFrClientFindUserByPart;
  packet_dto.structDTOClassType = StructDTOClassType::userLoginPasswordDTO;
  packet_dto.reqDirection = RequestDirection::ClientToSrv;
  packet_dto.structDTOPtr = std::make_shared<StructDTOClass<UserLoginPasswordDTO>>(user_login_password_dto);

  std::vector<PacketDTO> packet_list_send;
  packet_list_send.push_back(packet_dto);

  PacketListDTO response_packet_list;
  response_packet_list.packets.clear();

  response_packet_list = processingRequestToServerCore(packet_list_send, packet_dto.requestType);

  std::vector<UserDTO> result;
  result.clear();

  for (const auto &packet : response_packet_list.packets) {
    if (packet.requestType != RequestType::RqFrClientFindUserByPart) {
      continue;
    }

    if (!packet.structDTOPtr) {
      continue;
    }

    switch (packet.structDTOClassType) {
    case StructDTOClassType::userDTO: {
      const auto &packet_user_dto =
          static_cast<const StructDTOClass<UserDTO> &>(*packet.structDTOPtr).getStructDTOClass();
      result.push_back(packet_user_dto);
      break;
    }
    case StructDTOClassType::responceDTO: {
      const auto &response = static_cast<const StructDTOClass<ResponceDTO> &>(*packet.structDTOPtr).getStructDTOClass();
      if (!response.reqResult) {
        result.clear();
        return result;
      }
      break;
    }
    default:
      break;
    }
  }
  return result;
}

bool ClientCore::checkUserLoginCl(const std::string &user_login) {
  const auto is_on_client_device = chat_system_.findUserByLogin(user_login);
  if (is_on_client_device != nullptr) {
    return true;
  }

  UserLoginDTO user_login_dto;
  user_login_dto.login = user_login;

  PacketDTO packet_dto;
  packet_dto.requestType = RequestType::RqFrClientCheckLogin;
  packet_dto.structDTOClassType = StructDTOClassType::userLoginDTO;
  packet_dto.reqDirection = RequestDirection::ClientToSrv;
  packet_dto.structDTOPtr = std::make_shared<StructDTOClass<UserLoginDTO>>(user_login_dto);

  std::vector<PacketDTO> packet_list_send;
  packet_list_send.push_back(packet_dto);

  PacketListDTO packet_list_result;
  packet_list_result.packets.clear();

  packet_list_result = processingRequestToServerCore(packet_list_send, packet_dto.requestType);

  try {
    if (packet_list_result.packets.size() != 1) {
      throw exc_qt::WrongPacketSizeException();
    }

    if (packet_list_result.packets[0].requestType != RequestType::RqFrClientCheckLogin) {
      throw exc_qt::WrongresponceTypeException();
    }

    const auto &response = static_cast<const StructDTOClass<ResponceDTO> &>(*packet_list_result.packets[0].structDTOPtr)
                               .getStructDTOClass();
    return response.reqResult;
  } catch (const std::exception &ex) {
    const auto time_stamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto time_stamp_qt = QString::fromStdString(time_stamp);
    const auto user_login_qt = QString::fromStdString(user_login);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=]   [msg=]   checkUserLoginCl")
                                         .arg(time_stamp_qt, user_login_qt));
    return false;
  }
}

bool ClientCore::checkUserPasswordCl(const std::string &user_login, const std::string &password_plain) {
  const auto is_on_client_device = chat_system_.findUserByLogin(user_login);
  const auto password_hash = picosha2::hash256_hex_string(password_plain);

  if (is_on_client_device != nullptr) {
    return chat_system_.checkPasswordValidForUser(password_hash, user_login);
  }

  UserLoginPasswordDTO user_login_password_dto;
  user_login_password_dto.login = user_login;
  user_login_password_dto.passwordhash = password_hash;

  PacketDTO packet_dto;
  packet_dto.requestType = RequestType::RqFrClientCheckLogPassword;
  packet_dto.structDTOClassType = StructDTOClassType::userLoginPasswordDTO;
  packet_dto.reqDirection = RequestDirection::ClientToSrv;
  packet_dto.structDTOPtr = std::make_shared<StructDTOClass<UserLoginPasswordDTO>>(user_login_password_dto);

  std::vector<PacketDTO> packet_list_send;
  packet_list_send.push_back(packet_dto);

  PacketListDTO packet_list_result;
  packet_list_result.packets.clear();

  packet_list_result = processingRequestToServerCore(packet_list_send, packet_dto.requestType);
  if (packet_list_result.packets.empty()) {
    return false;
  }

  try {
    if (packet_list_result.packets.size() != 1) {
      throw exc_qt::WrongPacketSizeException();
    }

    if (packet_list_result.packets[0].requestType != RequestType::RqFrClientCheckLogPassword) {
      throw exc_qt::WrongresponceTypeException();
    }

    const auto &response = static_cast<const StructDTOClass<ResponceDTO> &>(*packet_list_result.packets[0].structDTOPtr)
                               .getStructDTOClass();
    return response.reqResult;
  } catch (const std::exception &ex) {
    const auto time_stamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto time_stamp_qt = QString::fromStdString(time_stamp);
    const auto user_login_qt = QString::fromStdString(user_login);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   checkUserPasswordCl")
                                         .arg(time_stamp_qt, user_login_qt));
    return false;
  }
}

bool ClientCore::findServerAddressCore(ServerConnectionConfig &config, ServerConnectionMode &mode) {
  const bool found = transport_.findServerAddress(config, mode);
  if (found) {
    server_connection_config_ = config;
    server_connection_mode_ = mode;
  }
  return found;
}

int ClientCore::createConnectionCore(const ServerConnectionConfig &config, ServerConnectionMode mode) {
  const int fd = transport_.createConnection(config, mode);
  if (fd >= 0) {
    server_connection_mode_ = mode;
    setSocketFdCore(fd);
  } else {
    updateConnectionState(false, ServerConnectionMode::Offline);
  }
  return fd;
}

bool ClientCore::discoverServerOnLANCore(ServerConnectionConfig &config) {
  return transport_.discoverServerOnLAN(config);
}

PacketListDTO ClientCore::getDatafromServerCore(const std::vector<std::uint8_t> &payload) {
  const int socket_fd = getSocketFdCore();
  if (socket_fd < 0) {
    PacketListDTO empty;
    empty.packets.clear();
    return empty;
  }
  return transport_.getDataFromServer(socket_fd, status_online_, payload);
}

PacketListDTO ClientCore::processingRequestToServerCore(std::vector<PacketDTO> &packets, const RequestType &request_type) {
  const int socket_fd = getSocketFdCore();
  if (socket_fd < 0) {
    PacketListDTO empty;
    empty.packets.clear();
    return empty;
  }
  return request_dispatcher_.process(socket_fd, status_online_, packets, request_type);
}

bool ClientCore::initServerConnectionCore() {
  auto config = server_connection_config_;
  auto mode = server_connection_mode_;

  if (!findServerAddressCore(config, mode)) {
    return false;
  }

  const int fd = createConnectionCore(config, mode);
  return fd >= 0;
}

void ClientCore::resetSessionDataCore() {
  chat_system_.setActiveUser(std::shared_ptr<User>{});
  chat_system_.setIsServerStatus(false);
  server_connection_config_ = {};
  server_connection_mode_ = ServerConnectionMode::Offline;
  setSocketFdCore(kInvalidSocket);
}

bool ClientCore::reInitilizeBaseCl() {
  UserLoginDTO user_login_dto;
  user_login_dto.login = "reInitilizeBase";

  PacketDTO packet_dto;
  packet_dto.reqDirection = RequestDirection::ClientToSrv;
  packet_dto.structDTOClassType = StructDTOClassType::userLoginDTO;
  packet_dto.requestType = RequestType::RqFrClientReInitializeBase;
  packet_dto.structDTOPtr = std::make_shared<StructDTOClass<UserLoginDTO>>(user_login_dto);

  std::vector<PacketDTO> packet_list_send;
  packet_list_send.push_back(packet_dto);

  PacketListDTO packet_list_result;
  packet_list_result.packets.clear();

  packet_list_result = processingRequestToServerCore(packet_list_send, packet_dto.requestType);

  try {
    if (packet_list_result.packets.empty()) {
      throw exc_qt::WrongresponceTypeException();
    }

    if (packet_list_result.packets[0].requestType != packet_dto.requestType) {
      throw exc_qt::WrongresponceTypeException();
    }

    const auto &packet = static_cast<const StructDTOClass<ResponceDTO> &>(*packet_list_result.packets[0].structDTOPtr)
                             .getStructDTOClass();
    return packet.reqResult;
  } catch (const exc_qt::WrongresponceTypeException &ex) {
    const auto time_stamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto time_stamp_qt = QString::fromStdString(time_stamp);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=]   [chat_Id=]   [msg=]   reInitilizeBaseCl   wrong type of answer from server")
                                         .arg(time_stamp_qt));
    return false;
  }
}

bool ClientCore::registerClientToSystemCl(const std::string &login) {
  UserLoginDTO user_login_dto;
  user_login_dto.login = login;

  PacketDTO packet_dto;
  packet_dto.reqDirection = RequestDirection::ClientToSrv;
  packet_dto.structDTOClassType = StructDTOClassType::userLoginDTO;
  packet_dto.structDTOPtr = std::make_shared<StructDTOClass<UserLoginDTO>>(user_login_dto);
  packet_dto.requestType = RequestType::RqFrClientRegisterUser;

  std::vector<PacketDTO> packet_list_send;
  packet_list_send.push_back(packet_dto);

  PacketListDTO packet_list_result;
  packet_list_result.packets.clear();

  packet_list_result = processingRequestToServerCore(packet_list_send, packet_dto.requestType);

  try {
    for (const auto &packet : packet_list_result.packets) {
      if (packet.requestType != packet_dto.requestType) {
        throw exc_qt::WrongresponceTypeException();
      }
    }

    for (const auto &packet : packet_list_result.packets) {
      if (!packet.structDTOPtr) {
        continue;
      }

      switch (packet.structDTOClassType) {
      case StructDTOClassType::userDTO: {
        const auto &user_dto = static_cast<const StructDTOClass<UserDTO> &>(*packet.structDTOPtr).getStructDTOClass();
        dto_writer_.setActiveUserDTOFromSrv(user_dto);
        break;
      }
      case StructDTOClassType::chatDTO: {
        const auto &chat_dto = static_cast<const StructDTOClass<ChatDTO> &>(*packet.structDTOPtr).getStructDTOClass();
        dto_writer_.setOneChatDTOFromSrv(chat_dto);
        break;
      }
      case StructDTOClassType::messageChatDTO: {
        const auto &message_chat_dto =
            static_cast<const StructDTOClass<MessageChatDTO> &>(*packet.structDTOPtr).getStructDTOClass();
        dto_writer_.setOneChatMessageDTO(message_chat_dto);
        break;
      }
      case StructDTOClassType::responceDTO: {
        const auto &response = static_cast<const StructDTOClass<ResponceDTO> &>(*packet.structDTOPtr).getStructDTOClass();
        if (!response.reqResult) {
          return false;
        }
        break;
      }
      default:
        break;
      }
    }
  } catch (const std::exception &ex) {
    const auto time_stamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto time_stamp_qt = QString::fromStdString(time_stamp);
    const auto login_qt = QString::fromStdString(login);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   registerClientToSystemCl")
                                         .arg(time_stamp_qt, login_qt));
    return false;
  }

  return true;
}

bool ClientCore::changeUserDataCl(const UserDTO &user_dto) {
  return modify_objects_.changeUserDataCl(user_dto);
}

bool ClientCore::createUserCl(const UserDTO &user_dto) { return create_objects_.createUserCl(user_dto); }

bool ClientCore::CreateAndSendNewChatCl(std::shared_ptr<Chat> &chat_ptr, std::vector<UserDTO> &participants,
                                        Message &new_message) {
  bool result = true;

  ChatDTO chat_dto;
  chat_dto.chatId = 0;
  chat_dto.senderLogin = getActiveUserCl()->getLogin();

  for (const auto &participant : participants) {
    ParticipantsDTO participants_dto;
    participants_dto.login = participant.login;
    participants_dto.lastReadMessage = 0;
    participants_dto.deletedMessageIds.clear();
    participants_dto.deletedFromChat = false;
    chat_dto.participants.push_back(participants_dto);
  }

  MessageDTO message_dto;
  message_dto.messageId = 0;
  message_dto.chatId = 0;
  message_dto.senderLogin = chat_dto.senderLogin;
  message_dto.timeStamp = new_message.getTimeStamp();

  MessageContentDTO tem_content;
  tem_content.messageContentType = MessageContentType::Text;
  auto content_element = new_message.getContent().front();
  auto content_text_ptr = std::dynamic_pointer_cast<MessageContent<TextContent>>(content_element);

  if (content_text_ptr) {
    auto content_text = content_text_ptr->getMessageContent();
    tem_content.payload = content_text._text;
  }

  message_dto.messageContent.push_back(tem_content);

  MessageChatDTO message_chat_dto;
  message_chat_dto.chatId = message_dto.chatId;
  message_chat_dto.messageDTO.push_back(message_dto);

  result = createNewChatCl(chat_ptr, chat_dto, message_chat_dto);

  if (result) {
    const auto new_message_id = chat_ptr->getMessages().begin()->second->getMessageId();
    chat_ptr->addParticipant(getActiveUserCl(), new_message_id, false);

    for (const auto &participant : participants) {
      if (participant.login != getActiveUserCl()->getLogin()) {
        const auto &user_ptr = chat_system_.findUserByLogin(participant.login);

        if (user_ptr == nullptr) {
          auto new_user_ptr = std::make_shared<User>(UserData(participant.login, participant.userName, "-1",
                                                             participant.email, participant.phone,
                                                             participant.disable_reason, participant.is_active,
                                                             participant.disabled_at, participant.ban_until));

          chat_system_.addUserToSystem(new_user_ptr);
          chat_ptr->addParticipant(new_user_ptr, 0, false);
        } else {
          chat_ptr->addParticipant(user_ptr, 0, false);
        }
      }
    }

    chat_system_.addChatToInstance(chat_ptr);
  }

  return result;
}

bool ClientCore::createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chat_dto, MessageChatDTO &message_chat_dto) {
  return create_objects_.createNewChatCl(chat, chat_dto, message_chat_dto);
}

MessageDTO ClientCore::fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chat_id) {
  return dto_builder_.fillOneMessageDTOFromCl(message, chat_id);
}

std::size_t ClientCore::createMessageCl(const Message &message, std::shared_ptr<Chat> &chat_ptr,
                                        const std::shared_ptr<User> &user) {
  return create_objects_.createMessageCl(message, chat_ptr, user);
}

bool ClientCore::sendLastReadMessageFromClient(const std::shared_ptr<Chat> &chat_ptr, std::size_t message_id) {
  MessageDTO message_dto;
  message_dto.chatId = chat_ptr->getChatId();
  message_dto.messageId = message_id;
  if (const auto active_user = chat_system_.getActiveUser()) {
    message_dto.senderLogin = active_user->getLogin();
  }
  message_dto.messageContent = {};
  message_dto.timeStamp = 0;

  PacketDTO packet_dto;
  packet_dto.requestType = RequestType::RqFrClientSetLastReadMessage;
  packet_dto.structDTOClassType = StructDTOClassType::messageDTO;
  packet_dto.reqDirection = RequestDirection::ClientToSrv;
  packet_dto.structDTOPtr = std::make_shared<StructDTOClass<MessageDTO>>(message_dto);

  std::vector<PacketDTO> packet_list_send;
  packet_list_send.push_back(packet_dto);

  PacketListDTO response_packet_list;
  response_packet_list.packets.clear();

  response_packet_list = processingRequestToServerCore(packet_list_send, RequestType::RqFrClientSetLastReadMessage);

  try {
    if (response_packet_list.packets.empty()) {
      throw exc_qt::EmptyPacketException();
    }

    if (response_packet_list.packets[0].requestType != RequestType::RqFrClientSetLastReadMessage) {
      throw exc_qt::WrongresponceTypeException();
    }

    const auto &response = static_cast<const StructDTOClass<ResponceDTO> &>(*response_packet_list.packets[0].structDTOPtr)
                               .getStructDTOClass();

    if (!response.reqResult) {
      throw exc_qt::LastReadMessageException();
    }

    return true;
  } catch (const std::exception &ex) {
    const auto time_stamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto time_stamp_qt = QString::fromStdString(time_stamp);
    const auto sender_login_qt = QString::fromStdString(message_dto.senderLogin);
    const auto chat_id_qt = QString::number(static_cast<long long>(message_dto.chatId));
    const auto message_id_qt = QString::number(static_cast<long long>(message_dto.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   sendLastReadMessageFromClient")
                                         .arg(time_stamp_qt, sender_login_qt, chat_id_qt, message_id_qt));
    return false;
  }
}

bool ClientCore::ensureParticipantsAvailable(const std::vector<std::string> &participants) {
  try {
    bool need_request = false;
    std::vector<PacketDTO> packet_list_send;

    for (const auto &participant : participants) {
      const auto &user_ptr = chat_system_.findUserByLogin(participant);
      if (!user_ptr) {
        need_request = true;

        UserLoginDTO user_login_dto;
        user_login_dto.login = participant;

        PacketDTO packet_dto;
        packet_dto.requestType = RequestType::RqFrClientGetUsersData;
        packet_dto.structDTOClassType = StructDTOClassType::userLoginDTO;
        packet_dto.reqDirection = RequestDirection::ClientToSrv;
        packet_dto.structDTOPtr = std::make_shared<StructDTOClass<UserLoginDTO>>(user_login_dto);

        packet_list_send.push_back(packet_dto);
      }
    }

    if (need_request) {
      PacketListDTO packet_list_result;
      packet_list_result.packets.clear();

      packet_list_result = processingRequestToServerCore(packet_list_send, RequestType::RqFrClientGetUsersData);

      for (const auto &response_packet : packet_list_result.packets) {
        if (response_packet.requestType != RequestType::RqFrClientGetUsersData) {
          continue;
        }

        if (response_packet.structDTOClassType != StructDTOClassType::userDTO) {
          throw exc_qt::WrongresponceTypeException();
        }

        const auto &user_dto = static_cast<const StructDTOClass<UserDTO> &>(*response_packet.structDTOPtr)
                                   .getStructDTOClass();

        auto new_user = std::make_shared<User>(UserData(user_dto.login, user_dto.userName, "-1", user_dto.email,
                                                        user_dto.phone, user_dto.disable_reason, user_dto.is_active,
                                                        user_dto.disabled_at, user_dto.ban_until));

        chat_system_.addUserToSystem(new_user);
      }
    } else {
      const auto time_stamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
      const auto time_stamp_qt = QString::fromStdString(time_stamp);

      emit exc_qt::ErrorBus::i().error("",
                                       QStringLiteral(
                                           "[%1]   ensureParticipantsAvailable   server answer did not come")
                                           .arg(time_stamp_qt));

      for (const auto &packet_dto : packet_list_send) {
        const auto &user_login_dto = static_cast<const StructDTOClass<UserLoginDTO> &>(*packet_dto.structDTOPtr)
                                         .getStructDTOClass();

        auto new_user = std::make_shared<User>(
            UserData(user_login_dto.login, "Unknown", "-1", "Unknown", "Unknown", "", true, 0, 0));

        chat_system_.addUserToSystem(new_user);
      }
    }
  } catch (const exc_qt::WrongresponceTypeException &ex) {
    const auto time_stamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto time_stamp_qt = QString::fromStdString(time_stamp);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=]   ensureParticipantsAvailable   Wrong responce type")
                                         .arg(time_stamp_qt));

    return false;
  }
  return true;
}

std::optional<ChatDTO> ClientCore::fillChatDTOCl(const std::shared_ptr<Chat> &chat_ptr) {
  return dto_builder_.fillChatDTOCl(chat_ptr);
}

bool ClientCore::blockUnblockUserCl(const std::string &login, bool is_blocked, const std::string &disable_reason) {
  const auto user_ptr = chat_system_.findUserByLogin(login);

  try {
    if (!user_ptr) {
      throw exc::UserNotFoundException();
    }

    UserDTO user_dto{};
    user_dto.login = login;
    user_dto.userName = "";
    user_dto.email = "";
    user_dto.phone = "";
    user_dto.passwordhash = "";
    user_dto.ban_until = 0;
    user_dto.disabled_at = 0;

    PacketDTO packet_dto;
    packet_dto.structDTOClassType = StructDTOClassType::userDTO;
    packet_dto.reqDirection = RequestDirection::ClientToSrv;

    if (is_blocked) {
      user_dto.is_active = false;
      user_dto.disable_reason = disable_reason;
      packet_dto.requestType = RequestType::RqFrClientBlockUser;
    } else {
      user_dto.is_active = true;
      user_dto.disable_reason = "";
      packet_dto.requestType = RequestType::RqFrClientUnBlockUser;
    }

    packet_dto.structDTOPtr = std::make_shared<StructDTOClass<UserDTO>>(user_dto);

    std::vector<PacketDTO> packet_list_send;
    packet_list_send.push_back(packet_dto);

    PacketListDTO packet_list_result;
    packet_list_result.packets.clear();

    packet_list_result = processingRequestToServerCore(packet_list_send, packet_dto.requestType);

    const auto &packet = static_cast<const StructDTOClass<ResponceDTO> &>(*packet_list_result.packets[0].structDTOPtr)
                             .getStructDTOClass();

    return packet.reqResult;
  } catch (const exc::UserNotFoundException &ex) {
    const auto time_stamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto time_stamp_qt = QString::fromStdString(time_stamp);
    const auto login_qt = QString::fromStdString(login);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [AUTH]   [user=%2]   blockUnblockUserCl")
                                         .arg(time_stamp_qt, login_qt));
    return false;
  }
}

bool ClientCore::bunUnbunUserCl(const std::string & /*login*/, bool /*is_banned*/, std::int64_t /*banned_to*/) {
  return false;
}

bool ClientCore::changeUserPasswordCl(const UserDTO & /*user_dto*/) { return false; }

void ClientCore::updateConnectionState(bool online, ServerConnectionMode mode) {
  const bool previous = status_online_.exchange(online, std::memory_order_acq_rel);
  chat_system_.setIsServerStatus(online);

  if (previous != online || server_connection_mode_ != mode) {
    server_connection_mode_ = mode;
    emit serverStatusChanged(online, mode);
  }
}
