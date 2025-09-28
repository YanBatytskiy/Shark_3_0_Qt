#include "client_session.h"

#include <poll.h>
#include <unistd.h>

#include <QString>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "chat/chat.h"
#include "chat_system/chat_system.h"
#include "client/core/client_core.h"
#include "dto/dto_struct.h"
#include "exceptions_cpp/login_exception.h"
#include "exceptions_qt/exception_login.h"
#include "exceptions_qt/exception_network.h"
#include "exceptions_qt/exception_router.h"
#include "message/message_content_struct.h"
#include "system/date_time_utils.h"
#include "system/serialize.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"

#ifndef POLLRDHUP
#define POLLRDHUP 0
#endif

// constructor
ClientSession::ClientSession(ChatSystem &chat_system, QObject *parent)
    : QObject(parent),
      _instance(chat_system),
      _core(chat_system, this),
      _dtoWriter(*this),
      _dtoBuilder(*this),
      _createObjects(*this),
      _modifyObjects(*this) {
  QObject::connect(&_core, &ClientCore::serverStatusChanged, this,
                   &ClientSession::serverStatusChanged);
}

ClientSession::~ClientSession() { stopConnectionThreadCl(); }

bool ClientSession::getIsServerOnlineCl() const noexcept {
  return _core.getIsServerOnlineCore();
}

bool ClientSession::inputNewLoginValidationQtCl(std::string inputData) {
  // проверяем только на англ буквы и цифры
  if (!engAndFiguresCheck(inputData))
    return false;
  else
    return true;
}

bool ClientSession::inputNewPasswordValidationQtCl(std::string inputData,
                                                   std::size_t dataLengthMin,
                                                   std::size_t dataLengthMax) {
  // проверяем только на англ буквы и цифры
  if (!engAndFiguresCheck(inputData)) return false;

  if (!checkNewLoginPasswordForLimits(inputData, dataLengthMin, dataLengthMax,
                                      true))
    return false;
  else
    return true;
}

std::optional<std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>>>
ClientSession::getChatListCl() {
  const auto &active_user = _instance.getActiveUser();
  if (!active_user) {
    return std::nullopt;
  }

  const auto &chat_list = active_user->getUserChatList();
  if (!chat_list) {
    return std::nullopt;
  }

  auto chats = chat_list->getChatFromList();

  std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>> result;

  if (chats.empty()) {
    return result;
  }

  for (const auto &chat : chats) {
    auto chat_ptr = chat.lock();
    if (!chat_ptr) {
      continue;
    }

    auto chat_dto = _dtoBuilder.fillChatDTOProcessing(chat_ptr);
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

// bool ClientSession::CreateAndSendNewChatQt(std::shared_ptr<Chat> &chat_ptr,
// std::vector<std::string> &participants, Message &newMessage) {
bool ClientSession::CreateAndSendNewChatCl(std::shared_ptr<Chat> &chat_ptr,
                                           std::vector<UserDTO> &participants,
                                           Message &newMessage) {
  bool result = true;

  ChatDTO chatDTO;
  chatDTO.chatId = 0;
  chatDTO.senderLogin = getActiveUserCl()->getLogin();

  for (const auto &participant : participants) {
    // временная структура для заполнения
    ParticipantsDTO participantsDTO;

    // заполняем данные на юзера для регистрации в системе
    participantsDTO.login = participant.login;

    // заполняем lastReadMessage
    participantsDTO.lastReadMessage = 0;

    // заполняем deletedMessageIds
    participantsDTO.deletedMessageIds.clear();

    participantsDTO.deletedFromChat = false;

    chatDTO.participants.push_back(participantsDTO);
  }  // for

  MessageDTO messageDTO;
  messageDTO.messageId = 0;
  messageDTO.chatId = 0;
  messageDTO.senderLogin = chatDTO.senderLogin;
  messageDTO.timeStamp = newMessage.getTimeStamp();

  // получаем контент
  MessageContentDTO temContent;
  temContent.messageContentType = MessageContentType::Text;
  auto contentElement = newMessage.getContent().front();

  auto contentTextPtr =
      std::dynamic_pointer_cast<MessageContent<TextContent>>(contentElement);

  if (contentTextPtr) {
    auto contentText = contentTextPtr->getMessageContent();
    temContent.payload = contentText._text;
  }

  messageDTO.messageContent.push_back(temContent);

  MessageChatDTO messageChatDTO;
  messageChatDTO.chatId = messageDTO.chatId;
  messageChatDTO.messageDTO.push_back(messageDTO);

  result = createNewChatCl(chat_ptr, chatDTO, messageChatDTO);

  // добавляем участников в чат
  if (result) {
    const auto newMessageId =
        chat_ptr->getMessages().begin()->second->getMessageId();

    chat_ptr->addParticipant(this->getActiveUserCl(), newMessageId, false);

    for (const auto &participant : participants) {
      if (participant.login != this->getActiveUserCl()->getLogin()) {
        const auto &user_ptr =
            this->getInstanceCl().findUserByLogin(participant.login);

        if (user_ptr == nullptr) {
          auto newUser_ptr = std::make_shared<User>(
              UserData(participant.login, participant.userName, "-1",
                       participant.email, participant.phone,
                       participant.disable_reason, participant.is_active,
                       participant.disabled_at, participant.ban_until));

          this->getInstanceCl().addUserToSystem(newUser_ptr);
          chat_ptr->addParticipant(newUser_ptr, 0, false);
        }  // if
        else
          chat_ptr->addParticipant(user_ptr, 0, false);
      }  // if activeuser
    }  // for

    // затем добавляем чат в систему
    this->getInstanceCl().addChatToInstance(chat_ptr);
  }  // if

  return result;
}

bool ClientSession::changeUserPasswordCl(UserDTO userDTO) {
  (void)userDTO;
  return false;
}

bool ClientSession::blockUnblockUserCl(const std::string &login, bool isBlocked,
                                       const std::string &disableReason) {
  const auto user_ptr = _instance.findUserByLogin(login);

  try {
    if (!user_ptr) throw exc::UserNotFoundException();

    UserDTO userDTO;

    userDTO.login = login;
    userDTO.userName = "";
    userDTO.email = "";
    userDTO.phone = "";
    userDTO.passwordhash = "";
    userDTO.ban_until = 0;
    userDTO.disabled_at = 0;

    PacketDTO packetDTO;
    packetDTO.structDTOClassType = StructDTOClassType::userDTO;
    packetDTO.reqDirection = RequestDirection::ClientToSrv;

    if (isBlocked) {
      userDTO.is_active = false;
      userDTO.disable_reason = disableReason;
      packetDTO.requestType = RequestType::RqFrClientBlockUser;
    } else {
      userDTO.is_active = true;
      userDTO.disable_reason = "";
      packetDTO.requestType = RequestType::RqFrClientUnBlockUser;
    }

    packetDTO.structDTOPtr = std::make_shared<StructDTOClass<UserDTO>>(userDTO);

    std::vector<PacketDTO> packetDTOListSend;
    packetDTOListSend.push_back(packetDTO);

    PacketListDTO packetListDTOresult;
    packetListDTOresult.packets.clear();

    packetListDTOresult =
        processingRequestToServerCl(packetDTOListSend, packetDTO.requestType);

    const auto &packet = static_cast<const StructDTOClass<ResponceDTO> &>(
                             *packetListDTOresult.packets[0].structDTOPtr)
                             .getStructDTOClass();

    if (packet.reqResult)
      return true;
    else
      return false;

  }  // try
  catch (const exc::UserNotFoundException &ex) {
    const auto time_sdtamp =
        formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto userLoginQt = QString::fromStdString(login);

    emit exc_qt::ErrorBus::i().error(
        QString::fromUtf8(ex.what()),
        QStringLiteral(
            "[%1]   [ERROR]   [AUTH]   [user=%2]   changeUserPasswordQt   ")
            .arg(timeStampQt, userLoginQt));
    return false;
  }
}

bool ClientSession::bunUnbunUserCl(const std::string &login, bool isBanned,
                                   std::int64_t bunnedTo) {
  (void)login;
  (void)isBanned;
  (void)bunnedTo;
  return false;
}

// threads
void ClientSession::startConnectionThreadCl() {
  if (connection_thread_running_.exchange(true, std::memory_order_acq_rel)) {
    return;
  }
  connection_thread_ = std::thread([this] { connectionMonitorLoopCl(); });
}

void ClientSession::stopConnectionThreadCl() {
  if (!connection_thread_running_.exchange(false, std::memory_order_acq_rel)) {
    return;
  }
  if (connection_thread_.joinable()) {
    connection_thread_.join();
  }
}

// getters

ServerConnectionConfig &ClientSession::getserverConnectionConfigCl() {
  return _core.getServerConnectionConfigCore();
}

const ServerConnectionConfig &ClientSession::getserverConnectionConfigCl()
    const {
  return _core.getServerConnectionConfigCore();
}

ServerConnectionMode &ClientSession::getserverConnectionModeCl() {
  return _core.getServerConnectionModeCore();
}

const ServerConnectionMode &ClientSession::getserverConnectionModeCl() const {
  return _core.getServerConnectionModeCore();
}

const std::shared_ptr<User> ClientSession::getActiveUserCl() const {
  return _instance.getActiveUser();
}

ChatSystem &ClientSession::getInstanceCl() { return _instance; }

std::size_t ClientSession::getSocketFdCl() const {
  return static_cast<std::size_t>(_core.getSocketFdCore());
}

void ClientSession::setActiveUserCl(const std::shared_ptr<User> &user) {
  _instance.setActiveUser(user);
}

void ClientSession::setSocketFdCl(int socket_fd) {
  _core.setSocketFdCore(socket_fd);
}

//
//
//
// checking and finding
//
//
//
const std::vector<UserDTO> ClientSession::findUserByTextPartOnServerCl(
    const std::string &text_to_find) {
  return _core.findUserByTextPartOnServerCore(text_to_find);
}
//
//
//
bool ClientSession::checkUserLoginCl(const std::string &user_login) {
  return _core.checkUserLoginCore(user_login);
}
//
//
//
bool ClientSession::checkUserPasswordCl(const std::string &user_login,
                                        const std::string &password) {
  return _core.checkUserPasswordCore(user_login, password);
}
// transport
//
//
bool ClientSession::findServerAddressCl(
    ServerConnectionConfig &server_connection_config,
    ServerConnectionMode &server_connection_mode) {
  return _core.findServerAddressCore(server_connection_config,
                                     server_connection_mode);
}
//
//
//

int ClientSession::createConnectionCl(
    ServerConnectionConfig &server_connection_config,
    ServerConnectionMode &server_connection_mode) {
  return _core.createConnectionCore(server_connection_config,
                                    server_connection_mode);
}
//
//
//
bool ClientSession::discoverServerOnLANCl(
    ServerConnectionConfig &server_connection_config) {
  return _core.discoverServerOnLANCore(server_connection_config);
}
//
//
//
PacketListDTO ClientSession::getDatafromServerCl(
    const std::vector<std::uint8_t> &packet_list_send) {
  return _core.getDatafromServerCore(packet_list_send);
}

//
//
PacketListDTO ClientSession::processingRequestToServerCl(
    std::vector<PacketDTO> &packet_dto_vector,
    const RequestType &request_type) {
  return _core.processingRequestToServerCore(packet_dto_vector, request_type);
}

//
//
//
// utilities

bool ClientSession::initServerConnectionCl() {
  return _core.initServerConnectionCore();
}

void ClientSession::resetSessionDataCl() { _core.resetSessionDataCore(); }

bool ClientSession::reInitilizeBaseCl() { return _core.reInitilizeBaseCore(); }

//
//
//
bool ClientSession::registerClientToSystemCl(const std::string &login) {
  UserLoginDTO user_login_dto;
  user_login_dto.login = login;

  PacketDTO packet_dto;
  packet_dto.reqDirection = RequestDirection::ClientToSrv;
  packet_dto.structDTOClassType = StructDTOClassType::userLoginDTO;
  packet_dto.structDTOPtr =
      std::make_shared<StructDTOClass<UserLoginDTO>>(user_login_dto);
  packet_dto.requestType = RequestType::RqFrClientRegisterUser;

  std::vector<PacketDTO> packet_list_send;
  packet_list_send.push_back(packet_dto);

  auto packet_list_result =
      processingRequestToServerCl(packet_list_send, packet_dto.requestType);

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
          const auto &user_dto =
              static_cast<const StructDTOClass<UserDTO> &>(*packet.structDTOPtr)
                  .getStructDTOClass();
          _dtoWriter.setActiveUserDTOFromSrvProcessing(user_dto);
          break;
        }
        case StructDTOClassType::chatDTO: {
          const auto &chat_dto =
              static_cast<const StructDTOClass<ChatDTO> &>(*packet.structDTOPtr)
                  .getStructDTOClass();
          _dtoWriter.setOneChatDTOFromSrvProcessing(chat_dto);
          break;
        }
        case StructDTOClassType::messageChatDTO: {
          const auto &message_chat_dto =
              static_cast<const StructDTOClass<MessageChatDTO> &>(
                  *packet.structDTOPtr)
                  .getStructDTOClass();
          _dtoWriter.setOneChatMessageDTOProcessing(message_chat_dto);
          break;
        }
        case StructDTOClassType::responceDTO: {
          const auto &response =
              static_cast<const StructDTOClass<ResponceDTO> &>(
                  *packet.structDTOPtr)
                  .getStructDTOClass();
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
    const auto time_stamp =
        formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto time_stamp_qt = QString::fromStdString(time_stamp);
    const auto login_qt = QString::fromStdString(login);

    emit exc_qt::ErrorBus::i().error(
        QString::fromUtf8(ex.what()),
        QStringLiteral("[%1]   [ERROR]   [NETWORK]   [user=%2]   "
                       "registerClientToSystemCl")
            .arg(time_stamp_qt, login_qt));
    return false;
  }

  return true;
}

//
//
//

bool ClientSession::changeUserDataCl(const UserDTO &user_dto) {
  return _modifyObjects.changeUserDataProcessing(user_dto);
}

bool ClientSession::createUserCl(const UserDTO &user_dto) {
  return _createObjects.createUserProcessing(user_dto);
}

bool ClientSession::createNewChatCl(std::shared_ptr<Chat> &chat,
                                    ChatDTO &chat_dto,
                                    MessageChatDTO &message_chat_dto) {
  return _createObjects.createNewChatProcessing(chat, chat_dto,
                                                message_chat_dto);
}
//
//
//
MessageDTO ClientSession::fillOneMessageDTOFromCl(
    const std::shared_ptr<Message> &message, std::size_t chat_id) {
  return _dtoBuilder.fillOneMessageDTOFromProcessing(message, chat_id);
}

//
//
//
std::size_t ClientSession::createMessageCl(const Message &message,
                                           std::shared_ptr<Chat> &chat_ptr,
                                           const std::shared_ptr<User> &user) {
  return _createObjects.createMessageProcessing(message, chat_ptr, user);
}

//
//
//
// отправка пакета LastReadMessage
bool ClientSession::sendLastReadMessageFromClientCl(
    const std::shared_ptr<Chat> &chat_ptr, std::size_t message_id) {
  return _core.sendLastReadMessageFromClientCore(chat_ptr, message_id);
}

bool ClientSession::checkAndAddParticipantToSystemCl(
    const std::vector<std::string> &participants) {
  return _core.ensureParticipantsAvailableCore(participants);
}

std::optional<ChatDTO> ClientSession::fillChatDTOCl(
    const std::shared_ptr<Chat> &chat_ptr) {
  return _dtoBuilder.fillChatDTOProcessing(chat_ptr);
}

void ClientSession::connectionMonitorLoopCl() {
  try {
    bool online = _core.getIsServerOnlineCore();

    while (connection_thread_running_.load(std::memory_order_acquire)) {
      if (!online) {
        auto &config = _core.getServerConnectionConfigCore();
        auto &mode_ref = _core.getServerConnectionModeCore();

        if (!_core.findServerAddressCore(config, mode_ref)) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          continue;
        }

        const int fd = _core.createConnectionCore(config, mode_ref);

        if (fd < 0) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          continue;
        }

        online = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        continue;
      }

      const int fd = _core.getSocketFdCore();
      if (!socketAliveCl(fd)) {
        if (fd >= 0) {
          ::close(fd);
        }
        _core.setSocketFdCore(-1);
        online = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        continue;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  } catch (const std::exception &) {
    connection_thread_running_.store(false, std::memory_order_release);
    return;
  }
}

bool ClientSession::socketAliveCl(int fd) {
  if (fd < 0) {
    return false;
  }

  pollfd descriptor{};
  descriptor.fd = fd;
  descriptor.events = POLLIN | POLLERR | POLLHUP | POLLRDHUP;

  const int result = ::poll(&descriptor, 1, 0);

  if (result < 0) {
    return false;
  }

  if (result == 0) {
    return true;
  }

  if (descriptor.revents & (POLLERR | POLLHUP | POLLRDHUP)) {
    return false;
  }

  return true;
}
