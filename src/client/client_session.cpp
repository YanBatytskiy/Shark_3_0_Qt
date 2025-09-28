#include "client_session.h"
#include "chat/chat.h"
#include "chat_system/chat_system.h"
#include "dto/dto_struct.h"
#include "exceptions_cpp/login_exception.h"
#include "message/message_content_struct.h"
#include "system/date_time_utils.h"
#include "system/serialize.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"

#include "exceptions_qt/exception_login.h"
#include "exceptions_qt/exception_network.h"
#include "exceptions_qt/exception_router.h"
#include <QString>

#include "system/picosha2.h"
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <netinet/in.h>

// constructor
ClientSession::ClientSession(ChatSystem &client, QObject *parent)
    : QObject(parent), _instance(client), _core(client, this) {
  QObject::connect(&_core, &ClientCore::serverStatusChanged, this, &ClientSession::serverStatusChanged);
}

// qt methods

// itilities

bool ClientSession::reInitilizeBaseQt() {
  return reInitilizeBaseCl();
}

bool ClientSession::checkLoginQt(std::string login) {

  return checkUserLoginCl(login);
}

bool ClientSession::checkLoginPsswordQt(std::string login, std::string password) {

  auto passHash = picosha2::hash256_hex_string(password);

  return checkUserPasswordCl(login, passHash);
}

bool ClientSession::registerClientOnDeviceQt(std::string login) { return registerClientToSystemCl(login); }

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

bool ClientSession::createUserQt(UserDTO userDTO) {
  if (createUserCl(userDTO))
    return true;
  else
    return false;
}

bool ClientSession::changeUserDataQt(UserDTO userDTO) {
  return changeUserDataCl(userDTO);
}

bool ClientSession::changeUserPasswordQt(UserDTO userDTO) {
  (void)userDTO;
  return false;
}

bool ClientSession::blockUnblockUserQt(std::string login, bool isBlocked, std::string disableReason) {

  const auto user_ptr = _instance.findUserByLogin(login);

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
void ClientSession::startConnectionThread() { _core.startConnectionThreadCore(); }

void ClientSession::stopConnectionThread() { _core.stopConnectionThreadCore(); }

// getters

ServerConnectionConfig &ClientSession::getserverConnectionConfigCl() {
  return _core.getServerConnectionConfigCore();
}

const ServerConnectionConfig &ClientSession::getserverConnectionConfigCl() const {
  return _core.getServerConnectionConfigCore();
}

ServerConnectionMode &ClientSession::getserverConnectionModeCl() { return _core.getServerConnectionModeCore(); }

const ServerConnectionMode &ClientSession::getserverConnectionModeCl() const {
  return _core.getServerConnectionModeCore();
}

const std::shared_ptr<User> ClientSession::getActiveUserCl() const { return _instance.getActiveUser(); }

ChatSystem &ClientSession::getInstance() { return _instance; }

std::size_t ClientSession::getSocketFd() const { return static_cast<std::size_t>(_core.getSocketFdCore()); }

// setters

void ClientSession::setActiveUserCl(const std::shared_ptr<User> &user) { _instance.setActiveUser(user); }

void ClientSession::setSocketFd(int socketFd) { _core.setSocketFdCore(socketFd); };

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

  const auto isOnClientDevice = _instance.findUserByLogin(userLogin);

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

  const auto isOnClientDevice = _instance.findUserByLogin(userLogin);

  if (isOnClientDevice != nullptr)
    return _instance.checkPasswordValidForUser(passwordHash, userLogin);

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
bool ClientSession::findServerAddress(ServerConnectionConfig &serverConnectionConfig,
                                      ServerConnectionMode &serverConnectionMode) {
  return _core.findServerAddressCore(serverConnectionConfig, serverConnectionMode);
}
//
//
//

int ClientSession::createConnection(ServerConnectionConfig &serverConnectionConfig,
                                    ServerConnectionMode &serverConnectionMode) {
  return _core.createConnectionCore(serverConnectionConfig, serverConnectionMode);
}
//
//
//
bool ClientSession::discoverServerOnLAN(ServerConnectionConfig &serverConnectionConfig) {
  return _core.discoverServerOnLANCore(serverConnectionConfig);
}
//
//
//
PacketListDTO ClientSession::getDatafromServer(const std::vector<std::uint8_t> &packetListSend) {
  return _core.getDatafromServerCore(packetListSend);
}

//
//
PacketListDTO ClientSession::processingRequestToServer(std::vector<PacketDTO> &packetDTOListVector,
                                                       const RequestType &requestType) {
  return _core.processingRequestToServerCore(packetDTOListVector, requestType);
}

//
//
//
// utilities

bool ClientSession::initServerConnection() { return _core.initServerConnectionCore(); }

void ClientSession::resetSessionData() { _core.resetSessionDataCore(); }

bool ClientSession::reInitilizeBaseCl() {
  UserLoginDTO userLoginDTO;
  userLoginDTO.login = "reInitilizeBase";
  PacketDTO packetDTO;
  packetDTO.reqDirection = RequestDirection::ClientToSrv;
  packetDTO.structDTOClassType = StructDTOClassType::userLoginDTO;

  packetDTO.structDTOPtr = std::make_shared<StructDTOClass<UserLoginDTO>>(userLoginDTO);

  packetDTO.requestType = RequestType::RqFrClientReInitializeBase;

  std::vector<PacketDTO> packetDTOListSend;
  packetDTOListSend.push_back(packetDTO);

  PacketListDTO packetListDTOresult;
  packetListDTOresult.packets.clear();

  packetListDTOresult = processingRequestToServer(packetDTOListSend, packetDTO.requestType);

  try {

    if (packetListDTOresult.packets.empty())
      throw exc_qt::WrongresponceTypeException();

    if (packetListDTOresult.packets[0].requestType != packetDTO.requestType)
      throw exc_qt::WrongresponceTypeException();

    const auto &packet = static_cast<const StructDTOClass<ResponceDTO> &>(*packetListDTOresult.packets[0].structDTOPtr)
                             .getStructDTOClass();

    if (packet.reqResult)
      return true;
    else
      return false;
  } // try
  catch (const exc_qt::WrongresponceTypeException &ex) {

    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=]   [chat_Id=]   [msg=]   reInitilizeBaseCl   wrong type of answer from server")
                                         .arg(timeStampQt));
    return false;
  }
}

//
//
//
bool ClientSession::registerClientToSystemCl(const std::string &login) {
  // в данном методе мы передаем серверу логин и в ответ получаем
  // данные юзера, все его чаты и сообщения

  UserLoginDTO userLoginDTO;
  userLoginDTO.login = login;
  PacketDTO packetDTO;
  packetDTO.reqDirection = RequestDirection::ClientToSrv;
  packetDTO.structDTOClassType = StructDTOClassType::userLoginDTO;

  packetDTO.structDTOPtr = std::make_shared<StructDTOClass<UserLoginDTO>>(userLoginDTO);

  const auto isOnClientDevice = _instance.findUserByLogin(login);

  packetDTO.requestType = RequestType::RqFrClientRegisterUser;

  std::vector<PacketDTO> packetDTOListSend;
  packetDTOListSend.push_back(packetDTO);

  PacketListDTO packetListDTOresult;
  packetListDTOresult.packets.clear();

  packetListDTOresult = processingRequestToServer(packetDTOListSend, packetDTO.requestType);

  try {

    for (const auto &packet : packetListDTOresult.packets) {
      // std::cout << "packet type: " << static_cast<int>(packet.requestTyfpe)
      //           << std::endl;
      if (packet.requestType != packetDTO.requestType)
        throw exc_qt::WrongresponceTypeException();
    }

    for (const auto &packet : packetListDTOresult.packets) {

      switch (packet.structDTOClassType) {
      case StructDTOClassType::userDTO: {

        // достаем из указателя ссылку на структуру
        const auto &type = static_cast<const StructDTOClass<UserDTO> &>(*packet.structDTOPtr).getStructDTOClass();

        setActiveUserDTOFromSrv(type);
        break;
      }
      case StructDTOClassType::chatDTO: {
        // достаем из указателя ссылку на структуру
        const auto &type = static_cast<const StructDTOClass<ChatDTO> &>(*packet.structDTOPtr).getStructDTOClass();

        setOneChatDTOFromSrv(type);
        break;
      }
      case StructDTOClassType::messageChatDTO: {
        // достаем из указателя ссылку на структуру
        const auto &type = static_cast<const StructDTOClass<MessageChatDTO> &>(*packet.structDTOPtr)
                               .getStructDTOClass();

        setOneChatMessageDTO(type);
        break;
      }
      default:
        throw exc_qt::WrongresponceTypeException();
      } // switch
    } // for

  } // try
  catch (const exc_qt::WrongresponceTypeException &ex) {

    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto loginQt = QString::fromStdString(login);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   registerClientToSystemCl   wrong type packet from server")
                                         .arg(timeStampQt, loginQt));
    return false;
  } catch (const exc_qt::NetworkException &ex) {

    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto loginQt = QString::fromStdString(login);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   registerClientToSystemCl   uknown mistake")
                                         .arg(timeStampQt, loginQt));
    return false;
  }

  return true;
}

//
//
//

bool ClientSession::changeUserDataCl(const UserDTO &userDTO) {

  PacketDTO packetDTO;
  packetDTO.requestType = RequestType::RqFrClientChangeUserData;
  packetDTO.structDTOClassType = StructDTOClassType::userDTO;
  packetDTO.reqDirection = RequestDirection::ClientToSrv;
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
}

bool ClientSession::createUserCl(const UserDTO &userDTO) {

  PacketDTO packetDTO;
  packetDTO.requestType = RequestType::RqFrClientCreateUser;
  packetDTO.structDTOClassType = StructDTOClassType::userDTO;
  packetDTO.reqDirection = RequestDirection::ClientToSrv;
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
}
//
//
//
bool ClientSession::createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chatDTO, MessageChatDTO &messageChatDTO) {

  // логика такая
  // формируем чат и сообщение в пакет и отправляем на сервер
  // в ответ мы получаем номера чата и сообщения
  // если все ок, то вводим чат и сообщение в систему

  PacketDTO chatPacket;
  chatPacket.requestType = RequestType::RqFrClientCreateChat;
  chatPacket.structDTOClassType = StructDTOClassType::chatDTO;
  chatPacket.reqDirection = RequestDirection::ClientToSrv;
  chatPacket.structDTOPtr = std::make_shared<StructDTOClass<ChatDTO>>(chatDTO);

  std::vector<PacketDTO> packetDTOListSend;
  packetDTOListSend.push_back(chatPacket);

  PacketDTO messagePacket;
  messagePacket.requestType = RequestType::RqFrClientCreateChat;
  messagePacket.structDTOClassType = StructDTOClassType::messageChatDTO;
  messagePacket.reqDirection = RequestDirection::ClientToSrv;
  messagePacket.structDTOPtr = std::make_shared<StructDTOClass<MessageChatDTO>>(messageChatDTO);
  packetDTOListSend.push_back(messagePacket);

  PacketListDTO responcePacketListDTO;
  responcePacketListDTO.packets.clear();

  responcePacketListDTO = processingRequestToServer(packetDTOListSend, RequestType::RqFrClientCreateChat);

  try {
    if (responcePacketListDTO.packets.empty()) {
      throw exc_qt::EmptyPacketException();
    } else {
      if (responcePacketListDTO.packets[0].requestType != RequestType::RqFrClientCreateChat)
        throw exc_qt::WrongresponceTypeException();
      else {
        const auto &packetDTO = static_cast<const StructDTOClass<ResponceDTO> &>(
                                    *responcePacketListDTO.packets[0].structDTOPtr)
                                    .getStructDTOClass();

        if (!packetDTO.reqResult)
          throw exc_qt::CreateChatException();
        else {
          auto generalChatId = packetDTO.anyNumber;
          chat->setChatId(generalChatId);

          auto generalMessageId = parseGetlineToSizeT(packetDTO.anyString);

          const auto &temp = chat->getMessages();

          if (chat->getMessages().empty())
            throw exc_qt::CreateMessageException();

          chat->getMessages().begin()->second->setMessageId(generalMessageId);
        }
      }
    }
  } // try
  catch (const exc_qt::WrongresponceTypeException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::EmptyPacketException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::CreateChatException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::CreateChatIdException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::CreateMessageIdException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::CreateMessageException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  }

  return true;
}
//
//
//
MessageDTO ClientSession::fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chatId) {

  MessageDTO messageDTO;
  auto user_ptr = message->getSender().lock();

  messageDTO.chatId = chatId;
  messageDTO.messageId = message->getMessageId();
  messageDTO.senderLogin = user_ptr ? user_ptr->getLogin() : "";

  // получаем контент
  MessageContentDTO temContent;
  temContent.messageContentType = MessageContentType::Text;
  if (message->getContent().empty())
    throw exc_qt::UnknownException("Пустой контент сообщения");
  auto contentElement = message->getContent().front();

  auto contentTextPtr = std::dynamic_pointer_cast<MessageContent<TextContent>>(contentElement);

  if (contentTextPtr) {
    auto contentText = contentTextPtr->getMessageContent();
    temContent.payload = contentText._text;
  }
  messageDTO.messageContent.push_back(temContent);

  messageDTO.timeStamp = message->getTimeStamp();

  return messageDTO;
}
//
//
//
std::size_t ClientSession::createMessageCl(const Message &message, std::shared_ptr<Chat> &chat_ptr,
                                           const std::shared_ptr<User> &user) {

  auto message_ptr = std::make_shared<Message>(message);

  MessageDTO messageDTO = fillOneMessageDTOFromCl(message_ptr, chat_ptr->getChatId());

  PacketDTO packetDTO;
  packetDTO.requestType = RequestType::RqFrClientCreateMessage;
  packetDTO.structDTOClassType = StructDTOClassType::messageDTO;
  packetDTO.reqDirection = RequestDirection::ClientToSrv;
  packetDTO.structDTOPtr = std::make_shared<StructDTOClass<MessageDTO>>(messageDTO);

  std::vector<PacketDTO> packetDTOListForSendVector;
  packetDTOListForSendVector.push_back(packetDTO);

  PacketListDTO responcePacketListDTO;
  responcePacketListDTO.packets.clear();

  responcePacketListDTO = processingRequestToServer(packetDTOListForSendVector, packetDTO.requestType);

  std::size_t newMessageId = 0;

  try {

    if (responcePacketListDTO.packets.empty())
      throw exc_qt::EmptyPacketException();

    if (responcePacketListDTO.packets.size() > 1)
      throw exc_qt::WrongPacketSizeException();

    if (responcePacketListDTO.packets[0].requestType != RequestType::RqFrClientCreateMessage)
      throw exc_qt::WrongresponceTypeException();

    // доастали пакет
    const auto &packetDTO = static_cast<const StructDTOClass<ResponceDTO> &>(
                                *responcePacketListDTO.packets[0].structDTOPtr)
                                .getStructDTOClass();

    if (!packetDTO.reqResult)
      throw exc_qt::CreateMessageException();
    else {
      newMessageId = parseGetlineToSizeT(packetDTO.anyString);

      if (chat_ptr->getMessages().empty())
        throw exc_qt::CreateMessageException();

      message_ptr->setMessageId(newMessageId);

      chat_ptr->addMessageToChat(message_ptr, user, false);
    }
  } catch (const exc_qt::EmptyPacketException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createMessageCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return 0;
  } catch (const exc_qt::WrongPacketSizeException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createMessageCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return 0;
  } catch (const exc_qt::WrongresponceTypeException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createMessageCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return 0;
  } catch (const exc_qt::CreateMessageException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createMessageCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return 0;
  } catch (const exc_qt::ValidationException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createMessageCl   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return 0;
  }
  return newMessageId;
}
//
//
//
// отправка пакета LastReadMessage
bool ClientSession::sendLastReadMessageFromClient(const std::shared_ptr<Chat> &chat_ptr, std::size_t messageId) {
  // пакет для отправки
  MessageDTO messageDTO;
  messageDTO.chatId = chat_ptr->getChatId();
  messageDTO.messageId = messageId;
  messageDTO.senderLogin = _instance.getActiveUser()->getLogin();
  messageDTO.messageContent = {};
  messageDTO.timeStamp = 0;

  // отдельный пакет для отправки
  PacketDTO packetDTOForSend;
  packetDTOForSend.requestType = RequestType::RqFrClientSetLastReadMessage;
  packetDTOForSend.structDTOClassType = StructDTOClassType::messageDTO;
  packetDTOForSend.reqDirection = RequestDirection::ClientToSrv;
  packetDTOForSend.structDTOPtr = std::make_shared<StructDTOClass<MessageDTO>>(messageDTO);

  // создали структуру вектора пакетов для отправки
  std::vector<PacketDTO> packetDTOListSend;
  packetDTOListSend.push_back(packetDTOForSend);

  PacketListDTO responcePacketListDTO;
  responcePacketListDTO.packets.clear();

  responcePacketListDTO = processingRequestToServer(packetDTOListSend, RequestType::RqFrClientSetLastReadMessage);

  try {
    if (responcePacketListDTO.packets.empty()) {
      throw exc_qt::EmptyPacketException();
    } else {
      if (responcePacketListDTO.packets[0].requestType != RequestType::RqFrClientSetLastReadMessage)
        throw exc_qt::WrongresponceTypeException();
      else {
        const auto &packetDTO = static_cast<const StructDTOClass<ResponceDTO> &>(
                                    *responcePacketListDTO.packets[0].structDTOPtr)
                                    .getStructDTOClass();

        if (!packetDTO.reqResult)
          throw exc_qt::LastReadMessageException();
        else
          return true;
      }
    }
  } // try
  catch (const exc_qt::WrongresponceTypeException &ex) {

    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   sendLastReadMessageFromClient   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::EmptyPacketException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   sendLastReadMessageFromClient   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::LastReadMessageException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   sendLastReadMessageFromClient   ")
                                         .arg(timeStampQt, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  }
  return true;
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// transport setters
//
//
// получение пользователя
void ClientSession::setActiveUserDTOFromSrv(const UserDTO &userDTO) const {

  auto user_ptr = std::make_shared<User>(UserData(userDTO.login, userDTO.userName, userDTO.passwordhash, userDTO.email,
                                                  userDTO.phone, userDTO.disable_reason, userDTO.is_active,
                                                  userDTO.disabled_at, userDTO.ban_until));
  user_ptr->createChatList(std::make_shared<UserChatList>(user_ptr));

  if (!_instance.findUserByLogin(userDTO.login))
    _instance.addUserToSystem(user_ptr);

  _instance.setActiveUser(user_ptr);
}
//
//
//
void ClientSession::setUserDTOFromSrv(const UserDTO &userDTO) const {

  auto user_ptr = std::make_shared<User>(UserData(userDTO.login, userDTO.userName, "-1", userDTO.email, userDTO.phone,
                                                  userDTO.disable_reason, userDTO.is_active, userDTO.disabled_at,
                                                  userDTO.ban_until));
  user_ptr->createChatList();

  if (!_instance.findUserByLogin(userDTO.login))
    _instance.addUserToSystem(user_ptr);
}
//
//
// получение одного сообщения
void ClientSession::setOneMessageDTO(const MessageDTO &messageDTO, const std::shared_ptr<Chat> &chat) const {

  // получить указатель на пользователя - отправителя сообщения
  auto sender = this->_instance.findUserByLogin(messageDTO.senderLogin);

  // упрощенная передача только одного текстового сообщения

  auto message = createOneMessage(messageDTO.messageContent[0].payload, sender, messageDTO.timeStamp,
                                  messageDTO.messageId);

  chat->addMessageToChat(std::make_shared<Message>(message), sender, false);
}
//
//
// получение сообщений одного чата
bool ClientSession::setOneChatMessageDTO(const MessageChatDTO &messageChatDTO) const {

  // получили чат лист пользователя
  auto chats = this->_instance.getActiveUser()->getUserChatList()->getChatFromList();

  std::shared_ptr<Chat> chat_ptr;
  for (const auto &chat : chats) {
    chat_ptr = chat.lock();

    if (chat_ptr && chat_ptr->getChatId() == messageChatDTO.chatId) {

      // внутри найденного по chatId чата мы последовательно вызываем
      // заполнение одного сообщения
      for (const auto &message : messageChatDTO.messageDTO) {
        setOneMessageDTO(message, chat_ptr);
      }
    }
  }
  return true;
}
//
//
//
bool ClientSession::checkAndAddParticipantToSystem(const std::vector<std::string> &participants) {

  try {

    bool needRequest = false;
    std::vector<PacketDTO> packetDTOListSend;

    for (const auto &participant : participants) {

      const auto &user_ptr = this->_instance.findUserByLogin(participant);

      if (user_ptr == nullptr) {

        needRequest = true;

        UserLoginDTO userLoginDTO;
        userLoginDTO.login = participant;

        PacketDTO packetDTO;
        packetDTO.requestType = RequestType::RqFrClientGetUsersData;
        packetDTO.structDTOClassType = StructDTOClassType::userLoginDTO;
        packetDTO.reqDirection = RequestDirection::ClientToSrv;
        packetDTO.structDTOPtr = std::make_shared<StructDTOClass<UserLoginDTO>>(userLoginDTO);

        packetDTOListSend.push_back(packetDTO);
      } // if user_ptr
    }

    if (needRequest) {

      PacketListDTO packetListDTOresult;
      packetListDTOresult.packets.clear();

      packetListDTOresult = processingRequestToServer(packetDTOListSend, RequestType::RqFrClientGetUsersData);

      {
        // добавляем недостающих пользователей
        for (const auto &responcePacket : packetListDTOresult.packets) {
          if (responcePacket.requestType != RequestType::RqFrClientGetUsersData)
            continue; // Пропускаем чужие пакеты

          if (responcePacket.structDTOClassType != StructDTOClassType::userDTO)
            throw exc_qt::WrongresponceTypeException();

          const auto &userDTO = static_cast<const StructDTOClass<UserDTO> &>(*responcePacket.structDTOPtr)
                                    .getStructDTOClass();

          auto newUser_ptr = std::make_shared<User>(UserData(userDTO.login, userDTO.userName, "-1", userDTO.email,
                                                             userDTO.phone, userDTO.disable_reason, userDTO.is_active,
                                                             userDTO.disabled_at, userDTO.ban_until));

          _instance.addUserToSystem(newUser_ptr);
        }
      }
    } else { // если не пришел ответ с сервера
      const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
      const auto timeStampQt = QString::fromStdString(time_sdtamp);

      emit exc_qt::ErrorBus::i().error("",
                                       QStringLiteral(
                                           "[%1]   checkAndAddParticipantToSystem   server answer did not come")
                                           .arg(timeStampQt));

      for (const auto &packetDTO : packetDTOListSend) {

        const auto &userLoginDTO = static_cast<const StructDTOClass<UserLoginDTO> &>(*packetDTO.structDTOPtr)
                                       .getStructDTOClass();

        auto newUser_ptr = std::make_shared<User>(
            UserData(userLoginDTO.login, "Unknown", "-1", "Unknown", "Unknown", "", true, 0, 0));

        _instance.addUserToSystem(newUser_ptr);
      }
    }
  } // try
  catch (const exc_qt::WrongresponceTypeException &ex) {

    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=]   checkAndAddParticipantToSystem   Wrong responce type")
                                         .arg(timeStampQt));

    std::cerr << "Клиент: добавление участников. " << ex.what() << std::endl;
    return false;
  }
  return true;
}
//
//
//  получение чата
void ClientSession::setOneChatDTOFromSrv(const ChatDTO &chatDTO) {

  // инициализируем чат
  auto chat_ptr = std::make_shared<Chat>();
  auto chatList = this->getActiveUserCl()->getUserChatList();

  // добавляем поля
  chat_ptr->setChatId(chatDTO.chatId);

  // добавляем участников

  // сначала проверяем есть ли участники в системе и формируем запрос на
  // данные участников и запрашиваем недостающих с сервера

  std::vector<std::string> participants;
  participants.clear();

  for (const auto &participant : chatDTO.participants)
    participants.push_back(participant.login);

  checkAndAddParticipantToSystem(participants);

  for (const auto &participant : chatDTO.participants) {

    for (const auto &delMessId : participant.deletedMessageIds)
      chat_ptr->setDeletedMessageMap(participant.login, delMessId);

    auto user_ptr = this->_instance.findUserByLogin(participant.login);

    if (user_ptr) {
      chat_ptr->setLastReadMessageId(user_ptr, participant.lastReadMessage);
      chat_ptr->addParticipant(user_ptr, participant.lastReadMessage, participant.deletedFromChat);
    }
  }

  this->_instance.addChatToInstance(chat_ptr);
}

std::optional<ChatDTO> ClientSession::fillChatDTOQt(const std::shared_ptr<Chat> &chat_ptr) {
  ChatDTO chatDTO;

  // взяли chatId
  chatDTO.chatId = chat_ptr->getChatId();

  if (!_instance.getActiveUser())
    return std::nullopt;
  chatDTO.senderLogin = _instance.getActiveUser()->getLogin();

  // получаем список участников
  auto participants = chat_ptr->getParticipants();

  if (participants.empty())
    return std::nullopt;

  // перебираем участников
  for (const auto &participant : participants) {

    // получили указатель на юзера
    const auto user_ptr = participant._user.lock();

    // временная структура для заполнения
    ParticipantsDTO participantsDTO;

    if (user_ptr) {

      // заполняем данные на юзера для регистрации в системе

      participantsDTO.login = user_ptr->getLogin();

      // заполняем lastReadMessage
      participantsDTO.lastReadMessage = chat_ptr->getLastReadMessageId(user_ptr);

      // заполняем deletedMessageIds
      participantsDTO.deletedMessageIds.clear();
      const auto &delMessMap = chat_ptr->getDeletedMessagesMap();
      const auto &it = delMessMap.find(participantsDTO.login);

      if (it != delMessMap.end()) {
        for (const auto &delMessId : it->second) {
          participantsDTO.deletedMessageIds.push_back(delMessId);
        }
      }

      participantsDTO.deletedFromChat = false;

      chatDTO.participants.push_back(participantsDTO);

    } // if user_ptr
    else
      continue;
  } // for participants

  if (chatDTO.participants.empty())
    return std::nullopt;
  return chatDTO;
}
