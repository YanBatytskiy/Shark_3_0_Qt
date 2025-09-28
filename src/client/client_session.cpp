#include "client_session.h"

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
#include "system/picosha2.h"
#include "system/serialize.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"

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

#include <poll.h>
#include <unistd.h>

#ifndef POLLRDHUP
#define POLLRDHUP 0
#endif

// constructor
ClientSession::ClientSession(QObject *parent)
    : QObject(parent), chat_system_(std::make_unique<ChatSystem>()),
      client_core_(std::make_shared<ClientCore>(*chat_system_, this)) {
  QObject::connect(client_core_.get(), &ClientCore::serverStatusChanged, this, &ClientSession::serverStatusChanged);
}

ClientSession::~ClientSession() { stopConnectionThread(); }

bool ClientSession::getIsServerOnline() const noexcept { return client_core_->getIsServerOnlineCore(); }

bool ClientSession::inputNewLoginValidationQt(std::string inputData) {

  // проверяем только на англ буквы и цифры
  if (!engAndFiguresCheck(inputData))
    return false;
  else
    return true;
}

bool ClientSession::inputNewPasswordValidationQt(std::string inputData, std::size_t dataLengthMin,
                                                 std::size_t dataLengthMax) {

  // проверяем только на англ буквы и цифры
  if (!engAndFiguresCheck(inputData))
    return false;

  if (!checkNewLoginPasswordForLimits(inputData, dataLengthMin, dataLengthMax, true))
    return false;
  else
    return true;
}

std::optional<std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>>> ClientSession::getChatListQt() {

  if (!this->getActiveUserCl())
    return std::nullopt;
  if (!this->getActiveUserCl()->getUserChatList())
    return std::nullopt;

  auto chats = this->getActiveUserCl()->getUserChatList()->getChatFromList();

  // контейнер для сортировки чатов <время последнего сообщения, chatDTO
  // отсортирован в обратном порядке
  std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>> result;
  result.clear();

  if (chats.size() == 0) {
    return result;
  }

  for (const auto &chat : chats) {

    auto chat_ptr = chat.lock();

    if (chat_ptr) {

      auto chatDTO = fillChatDTOQt(chat_ptr);

      if (!chatDTO.has_value())
        continue;

      const auto &messages = chat_ptr->getMessages();
      std::int64_t lastMessageTimeStamp;

      if (messages.empty())
        lastMessageTimeStamp = 0;
      else
        lastMessageTimeStamp = messages.rbegin()->first;

      result.insert({lastMessageTimeStamp, chatDTO.value()});

    } // if chatPtr
  } // for

  return result;
}

// bool ClientSession::CreateAndSendNewChatQt(std::shared_ptr<Chat> &chat_ptr, std::vector<std::string> &participants, Message &newMessage) {
bool ClientSession::CreateAndSendNewChatQt(std::shared_ptr<Chat> &chat_ptr, std::vector<UserDTO> &participants, Message &newMessage) {

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
  } // for

  MessageDTO messageDTO;
  messageDTO.messageId = 0;
  messageDTO.chatId = 0;
  messageDTO.senderLogin = chatDTO.senderLogin;
  messageDTO.timeStamp = newMessage.getTimeStamp();

  // получаем контент
  MessageContentDTO temContent;
  temContent.messageContentType = MessageContentType::Text;
  auto contentElement = newMessage.getContent().front();

  auto contentTextPtr = std::dynamic_pointer_cast<MessageContent<TextContent>>(contentElement);

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

    const auto newMessageId = chat_ptr->getMessages().begin()->second->getMessageId();

    chat_ptr->addParticipant(this->getActiveUserCl(), newMessageId, false);

    for (const auto &participant : participants) {

      if (participant.login != this->getActiveUserCl()->getLogin()) {
        const auto &user_ptr = this->getInstance().findUserByLogin(participant.login);

        if (user_ptr == nullptr) {
          auto newUser_ptr = std::make_shared<User>(UserData(participant.login,
                                                             participant.userName,
                                                             "-1",
                                                             participant.email,
                                                             participant.phone,
                                                             participant.disable_reason,
                                                             participant.is_active,
                                                             participant.disabled_at,
                                                             participant.ban_until));

          this->getInstance().addUserToSystem(newUser_ptr);
          chat_ptr->addParticipant(newUser_ptr, 0, false);
        } // if
        else
          chat_ptr->addParticipant(user_ptr, 0, false);
      } // if activeuser
    } // for

    // затем добавляем чат в систему
    this->getInstance().addChatToInstance(chat_ptr);
  } // if

  return result;
}

bool ClientSession::changeUserPasswordQt(UserDTO userDTO) {
  (void)userDTO;
  return false;
}

bool ClientSession::blockUnblockUserQt(std::string login, bool isBlocked, std::string disableReason) {

  const auto user_ptr = getInstance().findUserByLogin(login);

  try {

    if (!user_ptr)
      throw exc::UserNotFoundException();

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

    packetListDTOresult = processingRequestToServer(packetDTOListSend, packetDTO.requestType);

    const auto &packet = static_cast<const StructDTOClass<ResponceDTO> &>(*packetListDTOresult.packets[0].structDTOPtr)
                             .getStructDTOClass();

    if (packet.reqResult)
      return true;
    else
      return false;

  } // try
  catch (const exc::UserNotFoundException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto userLoginQt = QString::fromStdString(login);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [AUTH]   [user=%2]   changeUserPasswordQt   ")
                                         .arg(timeStampQt, userLoginQt));
    return false;
  }
}

bool ClientSession::bunUnbunUserQt(std::string login, bool isBanned, std::int64_t bunnedTo) {
  (void)login;
  (void)isBanned;
  (void)bunnedTo;
  return false;
}

// threads
void ClientSession::startConnectionThread() {
  if (connection_thread_running_.exchange(true, std::memory_order_acq_rel)) {
    return;
  }
  connection_thread_ = std::thread([this] { connectionMonitorLoop(); });
}

void ClientSession::stopConnectionThread() {
  if (!connection_thread_running_.exchange(false, std::memory_order_acq_rel)) {
    return;
  }
  if (connection_thread_.joinable()) {
    connection_thread_.join();
  }
}

// getters

ServerConnectionConfig &ClientSession::getserverConnectionConfigCl() {
  return client_core_->getServerConnectionConfigCore();
}

const ServerConnectionConfig &ClientSession::getserverConnectionConfigCl() const {
  return client_core_->getServerConnectionConfigCore();
}

ServerConnectionMode &ClientSession::getserverConnectionModeCl() { return client_core_->getServerConnectionModeCore(); }

const ServerConnectionMode &ClientSession::getserverConnectionModeCl() const {
  return client_core_->getServerConnectionModeCore();
}

const std::shared_ptr<User> ClientSession::getActiveUserCl() const { return client_core_->getActiveUserCl(); }

ChatSystem &ClientSession::getInstance() { return client_core_->getInstance(); }

std::size_t ClientSession::getSocketFd() const {
  return static_cast<std::size_t>(client_core_->getSocketFdCore());
}

void ClientSession::setActiveUserCl(const std::shared_ptr<User> &user) { client_core_->setActiveUserCl(user); }

void ClientSession::setSocketFd(int socket_fd) { client_core_->setSocketFdCore(socket_fd); }

//
//
//
// checking and finding
//
//
//
const std::vector<UserDTO> ClientSession::findUserByTextPartOnServerCl(const std::string &textToFind) {

  UserLoginPasswordDTO userLoginPasswordDTO;
  userLoginPasswordDTO.login = this->getInstance().getActiveUser()->getLogin();
  userLoginPasswordDTO.passwordhash = textToFind;

  PacketDTO packetDTO;
  packetDTO.requestType = RequestType::RqFrClientFindUserByPart;
  packetDTO.structDTOClassType = StructDTOClassType::userLoginPasswordDTO;
  packetDTO.reqDirection = RequestDirection::ClientToSrv;
  packetDTO.structDTOPtr = std::make_shared<StructDTOClass<UserLoginPasswordDTO>>(userLoginPasswordDTO);

  std::vector<PacketDTO> packetDTOListSend;
  packetDTOListSend.push_back(packetDTO);

  PacketListDTO responcePacketListDTO;
  responcePacketListDTO.packets.clear();

  responcePacketListDTO = processingRequestToServer(packetDTOListSend, packetDTO.requestType);

  std::vector<UserDTO> result;
  result.clear();

  //   try {
  // if (!responcePacketListDTO.packets.empty()) {

  for (const auto &packet : responcePacketListDTO.packets) {

    if (packet.requestType != RequestType::RqFrClientFindUserByPart)
      //   throw exc_qt::WrongresponceTypeException();
      continue;
    // else {

    if (!packet.structDTOPtr)
      continue;

    switch (packet.structDTOClassType) {
    case StructDTOClassType::userDTO: {

      const auto &packetUserDTO = static_cast<const StructDTOClass<UserDTO> &>(*packet.structDTOPtr)
                                      .getStructDTOClass();
      result.push_back(packetUserDTO);
      break;
      // }
      //   }
    } // case
    case StructDTOClassType::responceDTO: {
      const auto &r =
          static_cast<const StructDTOClass<ResponceDTO> &>(*packet.structDTOPtr).getStructDTOClass();

      if (!r.reqResult) {
        result.clear(); // «не найдено»
        return result;  // ранний выход без падения
      }
    } // case
    default:
      break;
    } // switch
  }
  return result;
}
//
//
//
bool ClientSession::checkUserLoginCl(const std::string &userLogin) {

  const auto isOnClientDevice = getInstance().findUserByLogin(userLogin);

  if (isOnClientDevice != nullptr)
    return true;

  UserLoginDTO userLoginDTO;
  userLoginDTO.login = userLogin;

  PacketDTO packetDTO;
  packetDTO.requestType = RequestType::RqFrClientCheckLogin;
  packetDTO.structDTOClassType = StructDTOClassType::userLoginDTO;
  packetDTO.reqDirection = RequestDirection::ClientToSrv;
  packetDTO.structDTOPtr = std::make_shared<StructDTOClass<UserLoginDTO>>(userLoginDTO);

  std::vector<PacketDTO> packetDTOListSend;
  packetDTOListSend.push_back(packetDTO);

  PacketListDTO packetListDTOresult;
  packetListDTOresult.packets.clear();

  packetListDTOresult = processingRequestToServer(packetDTOListSend, packetDTO.requestType);

  //  std::vector<PacketDTO> responcePacketListDTO;
  try {
    if (packetListDTOresult.packets.size() != 1)
      throw exc_qt::WrongPacketSizeException();

    if (packetListDTOresult.packets[0].requestType != RequestType::RqFrClientCheckLogin)
      throw exc_qt::WrongresponceTypeException();
    else {
      const auto &responceDTO = static_cast<const StructDTOClass<ResponceDTO> &>(
                                    *packetListDTOresult.packets[0].structDTOPtr)
                                    .getStructDTOClass();

      return responceDTO.reqResult;
    }

  } catch (const exc_qt::WrongPacketSizeException &ex) {

    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto userLoginQt = QString::fromStdString(userLogin);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=]   [msg=]   checkUserLoginCl   wrong quantity packets in answer")
                                         .arg(timeStampQt, userLoginQt));
    return false;
  } catch (const std::exception &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto userLoginQt = QString::fromStdString(userLogin);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=]   [msg=]   checkUserLoginCl   uknown mistake")
                                         .arg(timeStampQt, userLoginQt));
    return false;
  }
  return true;
}
//
//
//
bool ClientSession::checkUserPasswordCl(const std::string &userLogin, const std::string &passwordHash) {

  const auto isOnClientDevice = getInstance().findUserByLogin(userLogin);

  if (isOnClientDevice != nullptr)
    return getInstance().checkPasswordValidForUser(passwordHash, userLogin);

  UserLoginPasswordDTO userLoginPasswordDTO;
  userLoginPasswordDTO.login = userLogin;
  userLoginPasswordDTO.passwordhash = passwordHash;

  PacketDTO packetDTO;
  packetDTO.requestType = RequestType::RqFrClientCheckLogPassword;
  packetDTO.structDTOClassType = StructDTOClassType::userLoginPasswordDTO;
  packetDTO.reqDirection = RequestDirection::ClientToSrv;

  packetDTO.structDTOPtr = std::make_shared<StructDTOClass<UserLoginPasswordDTO>>(userLoginPasswordDTO);

  std::vector<PacketDTO> packetDTOListSend;
  packetDTOListSend.push_back(packetDTO);

  PacketListDTO packetListDTOresult;
  packetListDTOresult.packets.clear();

  packetListDTOresult = processingRequestToServer(packetDTOListSend, packetDTO.requestType);
  if (packetListDTOresult.packets.size() == 0)
    return false;

  try {
    if (packetListDTOresult.packets.size() != 1)
      throw exc_qt::WrongPacketSizeException();

    if (packetListDTOresult.packets[0].requestType != RequestType::RqFrClientCheckLogPassword)
      throw exc_qt::WrongresponceTypeException();
    else {
      const auto &responceDTO = static_cast<const StructDTOClass<ResponceDTO> &>(
                                    *packetListDTOresult.packets[0].structDTOPtr)
                                    .getStructDTOClass();

      return responceDTO.reqResult;
    }

  } catch (const exc_qt::WrongPacketSizeException &ex) {

    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto userLoginQt = QString::fromStdString(userLogin);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   checkUserPasswordCl   ")
                                         .arg(timeStampQt, userLoginQt));
    return false;
  } catch (const exc_qt::WrongresponceTypeException &ex) {

    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto userLoginQt = QString::fromStdString(userLogin);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   checkUserPasswordCl   ")
                                         .arg(timeStampQt, userLoginQt));
    return false;
  } catch (const std::exception &ex) {

    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto userLoginQt = QString::fromStdString(userLogin);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2].  checkUserPasswordCl   ")
                                         .arg(timeStampQt, userLoginQt));
    return false;
  }
}
// transport
//
//
bool ClientSession::findServerAddress(ServerConnectionConfig &server_connection_config,
                                      ServerConnectionMode &server_connection_mode) {
  return client_core_->findServerAddressCore(server_connection_config, server_connection_mode);
}
//
//
//

int ClientSession::createConnection(ServerConnectionConfig &server_connection_config,
                                    ServerConnectionMode &server_connection_mode) {
  return client_core_->createConnectionCore(server_connection_config, server_connection_mode);
}
//
//
//
bool ClientSession::discoverServerOnLAN(ServerConnectionConfig &server_connection_config) {
  return client_core_->discoverServerOnLANCore(server_connection_config);
}
//
//
//
PacketListDTO ClientSession::getDatafromServer(const std::vector<std::uint8_t> &packet_list_send) {
  return client_core_->getDatafromServerCore(packet_list_send);
}

//
//
PacketListDTO ClientSession::processingRequestToServer(std::vector<PacketDTO> &packet_dto_vector,
                                                       const RequestType &request_type) {
  return client_core_->processingRequestToServerCore(packet_dto_vector, request_type);
}

//
//
//
// utilities

bool ClientSession::initServerConnection() { return client_core_->initServerConnectionCore(); }

void ClientSession::resetSessionData() { client_core_->resetSessionDataCore(); }

bool ClientSession::reInitilizeBaseCl() { return client_core_->reInitilizeBaseCl(); }

//
//
//
bool ClientSession::registerClientToSystemCl(const std::string &login) { return client_core_->registerClientToSystemCl(login); }


//
//
//

bool ClientSession::changeUserDataCl(const UserDTO &user_dto) { return client_core_->changeUserDataCl(user_dto); }

bool ClientSession::createUserCl(const UserDTO &user_dto) { return client_core_->createUserCl(user_dto); }

bool ClientSession::createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chat_dto, MessageChatDTO &message_chat_dto) {
  return client_core_->createNewChatCl(chat, chat_dto, message_chat_dto);
}
//
//
//
MessageDTO ClientSession::fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chat_id) { return client_core_->fillOneMessageDTOFromCl(message, chat_id); }

//
//
//
std::size_t ClientSession::createMessageCl(const Message &message, std::shared_ptr<Chat> &chat_ptr, const std::shared_ptr<User> &user) { return client_core_->createMessageCl(message, chat_ptr, user); }

//
//
//
// отправка пакета LastReadMessage
bool ClientSession::sendLastReadMessageFromClient(const std::shared_ptr<Chat> &chat_ptr, std::size_t message_id) { return client_core_->sendLastReadMessageFromClient(chat_ptr, message_id); }

bool ClientSession::checkAndAddParticipantToSystem(const std::vector<std::string> &participants) { return client_core_->ensureParticipantsAvailable(participants); }

std::optional<ChatDTO> ClientSession::fillChatDTOQt(const std::shared_ptr<Chat> &chat_ptr) { return client_core_->fillChatDTOQt(chat_ptr); }

std::shared_ptr<ClientCore> ClientSession::getClientCore() const { return client_core_; }

void ClientSession::connectionMonitorLoop() {
  try {
    bool online = client_core_->getIsServerOnlineCore();

    while (connection_thread_running_.load(std::memory_order_acquire)) {
      if (!online) {
        auto &config = client_core_->getServerConnectionConfigCore();
        auto &mode_ref = client_core_->getServerConnectionModeCore();

        if (!client_core_->findServerAddressCore(config, mode_ref)) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          continue;
        }

        const int fd = client_core_->createConnectionCore(config, mode_ref);

        if (fd < 0) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          continue;
        }

        online = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        continue;
      }

      const int fd = client_core_->getSocketFdCore();
      if (!socketAlive(fd)) {
        if (fd >= 0) {
          ::close(fd);
        }
        client_core_->setSocketFdCore(-1);
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

bool ClientSession::socketAlive(int fd) {
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
