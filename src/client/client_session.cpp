#include "client_session.h"
#include "chat/chat.h"
#include "chat_system/chat_system.h"
#include "dto/dto_struct.h"
#include "message/message_content_struct.h"
#include "system/serialize.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"

#include "exceptions_qt/exception_router.h"
#include "exceptions_qt/exception_login.h"
#include "exceptions_qt/exception_network.h"
#include "nw_connection_monitor.h"
#include <QCoreApplication>
#include <atomic>
#include <cerrno>
#include <qnamespace.h>
#include <qobject.h>
#include <qobjectdefs.h>

#include "system/picosha2.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <optional>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>

struct sockaddr_in serveraddress, client;
// constructor
ClientSession::ClientSession(ChatSystem &client, QObject *parent) : QObject(parent), _instance(client), _socketFd() {}

// qt methods

// itilities
bool ClientSession::checkLoginPsswordQt(std::string login, std::string password) {

  auto passHash = picosha2::hash256_hex_string(password);

  return checkUserPasswordCl(login, passHash);
}

bool ClientSession::registerClientOnDeviceQt(std::string login) { return registerClientToSystemCl(login); }

bool ClientSession::inputNewLoginValidationQt(std::string inputData, std::size_t dataLengthMin,
                                              std::size_t dataLengthMax) {

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

      auto chatDTO = FillForSendOneChatDTOFromClient(chat_ptr);

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

// threads
void ClientSession::startConnectionThread() {

  if (_connectionMonitor)
    return;

  _connectionMonitor = new ConnectionMonitor(this);
  _connectionMonitor->moveToThread(&_netThread);

  QObject::connect(&_netThread, &QThread::started, _connectionMonitor, &ConnectionMonitor::run);
  QObject::connect(&_netThread, &QThread::finished, _connectionMonitor, &QObject::deleteLater);

  QObject::connect(_connectionMonitor, &ConnectionMonitor::connectionStateChanged, this,
                   &ClientSession::onConnectionStateChanged, Qt::QueuedConnection);

  _netThread.start();
}

void ClientSession::stopConnectionThread() {

  if (!_connectionMonitor)
    return;

  _connectionMonitor->stop();
  _netThread.quit();
  _netThread.wait();
  _connectionMonitor = nullptr;
}

void ClientSession::onConnectionStateChanged(bool online, ServerConnectionMode mode) {
  _statusOnline.store(online, std::memory_order_release);
  _serverConnectionMode = mode;
  emit serverStatusChanged(online, mode);
}

// getters

ConnectionMonitor *ClientSession::getConnectionMonitor() { return _connectionMonitor; }

ServerConnectionConfig &ClientSession::getserverConnectionConfigCl() { return _serverConnectionConfig; }

const ServerConnectionConfig &ClientSession::getserverConnectionConfigCl() const { return _serverConnectionConfig; }

ServerConnectionMode &ClientSession::getserverConnectionModeCl() { return _serverConnectionMode; }

const ServerConnectionMode &ClientSession::getserverConnectionModeCl() const { return _serverConnectionMode; }

const std::shared_ptr<User> ClientSession::getActiveUserCl() const { return _instance.getActiveUser(); }

ChatSystem &ClientSession::getInstance() { return _instance; }

std::size_t ClientSession::getSocketFd() const { return _socketFd; }

// setters

void ClientSession::setActiveUserCl(const std::shared_ptr<User> &user) { _instance.setActiveUser(user); }

void ClientSession::setSocketFd(int socketFd) { _socketFd = socketFd; };

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

  try {
    if (!responcePacketListDTO.packets.empty()) {

      for (const auto &packet : responcePacketListDTO.packets) {

        if (packet.requestType != RequestType::RqFrClientFindUserByPart)
          throw exc_qt::WrongresponceTypeException();
        else {
          const auto &packetUserDTO = static_cast<const StructDTOClass<UserDTO> &>(*packet.structDTOPtr)
                                          .getStructDTOClass();
          result.push_back(packetUserDTO);
        }
      }
    } else
      throw exc_qt::WrongPacketSizeException();

  } catch (const exc_qt::WrongresponceTypeException &ex) {
    std::cout << "Клиент. Поиск пользователей по части слова. " << ex.what() << std::endl;
    result.clear();
  } catch (const std::exception &ex) {
    std::cout << "Клиент. Неизвестная ошибка. " << ex.what() << std::endl;
    result.clear();
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
    std::cout << "Клиент. Проверка логина. Неправильное количество пакетов в ответе." << ex.what() << std::endl;
    return false;
  } catch (const std::exception &ex) {
    std::cout << "Клиент. Неизвестная ошибка. " << ex.what() << std::endl;
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
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "Клиент. Проверка пароля. Неправильное количество пакетов в ответе."));

    throw;
  } catch (const exc_qt::WrongresponceTypeException &ex) {
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "Клиент. Проверка пароля. Неправильное тип пакета в ответе сервера."));

    throw;
  } catch (const std::exception &ex) {
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral("Клиент.Проверка пароля. Неизвестная ошибка."));
    throw;
  }
}
// transport
//
//
bool ClientSession::findServerAddress(ServerConnectionConfig &serverConnectionConfig,
                                      ServerConnectionMode &serverConnectionMode) {

  // открываем сокет для поиска
  int socketTmp = (socket(AF_INET, SOCK_STREAM, 0));

  try {
    if (socketTmp == -1) {
      throw exc_qt::CreateSocketTypeException();
    } else {
      // сначала ищем сервер на локальной машине
      sockaddr_in addr{};
      addr.sin_family = AF_INET;
      addr.sin_port = htons(serverConnectionConfig.port);

      addr.sin_addr.s_addr = inet_addr(serverConnectionConfig.addressLocalHost.c_str());

      int result = ::connect(socketTmp, (sockaddr *)&addr, sizeof(addr));

      if (result == 0) {
        serverConnectionConfig.found = true;
        serverConnectionMode = ServerConnectionMode::Localhost;

        close(socketTmp);
        return true;
      }

      // эатем пытаемся найти сервер внутри локальной сети
      discoverServerOnLAN(serverConnectionConfig);
      if (serverConnectionConfig.found) {
        serverConnectionMode = ServerConnectionMode::LocalNetwork;
        close(socketTmp);
        return true;
      }
    }
  } // try
  catch (const exc_qt::CreateSocketTypeException &ex) {
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()), QStringLiteral("Клиент. Поиск сервера."));
  }

  close(socketTmp);
  serverConnectionMode = ServerConnectionMode::Offline;
  return false;
}
//
//
//

int ClientSession::createConnection(ServerConnectionConfig &serverConnectionConfig,
                                    ServerConnectionMode &serverConnectionMode) {

  // Зададим номер порта
  serveraddress.sin_port = htons(serverConnectionConfig.port);
  // Используем IPv4
  serveraddress.sin_family = AF_INET;

  //  Установим адрес сервера
  switch (serverConnectionMode) {
  case ServerConnectionMode::Localhost: {
    serveraddress.sin_addr.s_addr = inet_addr(serverConnectionConfig.addressLocalHost.c_str());
    break;
  }
  case ServerConnectionMode::LocalNetwork: {
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int gaiResult = getaddrinfo(serverConnectionConfig.addressLocalNetwork.c_str(), nullptr, &hints, &res);
    if (gaiResult != 0 || res == nullptr) {
      serveraddress.sin_addr.s_addr = INADDR_NONE;
    } else {
      sockaddr_in *ipv4 = (sockaddr_in *)res->ai_addr;
      serveraddress.sin_addr = ipv4->sin_addr;
      freeaddrinfo(res);
    }
    break;
  }
  default:
    break;
  }

  // открываем сокет для поиска
  int socketTmp = (socket(AF_INET, SOCK_STREAM, 0));

  try {
    if (socketTmp == -1)
      throw exc_qt::CreateSocketTypeException();
    else {
      if (::connect(socketTmp, (sockaddr *)&serveraddress, sizeof(serveraddress)) < 0)
        throw exc_qt::ConnectionToServerException();
      else
        _socketFd = socketTmp;
    }
  } // try
  catch (const exc_qt::NetworkException &ex) {
    std::cerr << "Client: " << ex.what() << std::endl;
    close(socketTmp);
    return -1;
  }
  return _socketFd;
}
//
//
//
bool ClientSession::discoverServerOnLAN(ServerConnectionConfig &serverConnectionConfig) {
  int timeoutMs = 1000;

  try {
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket < 0) {
      throw exc_qt::CreateSocketTypeException();
    }

    // Включаем broadcast
    int broadcastEnable = 1;
    setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

    // Установка таймаута
    timeval timeout{};
    timeout.tv_sec = timeoutMs / 1000;
    timeout.tv_usec = (timeoutMs % 1000) * 1000;
    setsockopt(udpSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    sockaddr_in broadcastAddr{};
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(serverConnectionConfig.port);
    broadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");

    std::string ping = "ping?";
    sendto(udpSocket, ping.c_str(), ping.size(), 0, (sockaddr *)&broadcastAddr, sizeof(broadcastAddr));

    char buffer[128] = {0};
    sockaddr_in serverAddr{};
    socklen_t addrLen = sizeof(serverAddr);
    ssize_t bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr *)&serverAddr, &addrLen);

    if (bytesReceived > 0) {
      std::string msg(buffer);
      if (msg == "pong") {
        serverConnectionConfig.addressLocalNetwork = inet_ntoa(serverAddr.sin_addr);
        serverConnectionConfig.found = true;
      }
    }

    close(udpSocket);
    return true;
  } catch (const exc_qt::CreateSocketTypeException &ex) {

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()), QStringLiteral("Клиент. Поиск в LAN."));
    serverConnectionConfig.found = false;
    throw;
  }
}
//
//
//
PacketListDTO ClientSession::getDatafromServer(const std::vector<std::uint8_t> &packetListSend) {

  PacketListDTO packetListDTOresult;
  packetListDTOresult.packets.clear();

  try {

    if (!_statusOnline.load(std::memory_order_acquire))
      throw exc_qt::ConnectionToServerException();

    // проверка
    // for (std::uint8_t b : packetListSend) {
    //   std::cerr << static_cast<int>(b) << " ";
    // }
    // std::cerr << std::endl;

    // 1. Добавляем 4 байта длины в начало
    std::vector<std::uint8_t> packetWithSize;
    uint32_t len = htonl(static_cast<uint32_t>(packetListSend.size()));

    packetWithSize.resize(4 + packetListSend.size());
    std::memcpy(packetWithSize.data(), &len, 4); // первые 4 байта — длина
    std::memcpy(packetWithSize.data() + 4, packetListSend.data(), packetListSend.size()); // далее — данные

    // 2. Отправка
    // Очистка входящего буфера перед отправкой запроса

    // Сохраняем текущий режим сокета
    int flags = fcntl(_socketFd, F_GETFL, 0);
    fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK);

    // Очищаем входящий буфер
    std::vector<std::uint8_t> drainBuf(4096);
    try {
      for (;;) {
        ssize_t n = ::recv(_socketFd, drainBuf.data(), drainBuf.size(), MSG_DONTWAIT);
        if (n > 0)
          continue;
        if (n == 0) {
          _statusOnline.store(false, std::memory_order_release);
          throw exc_qt::ConnectionToServerException();
        }
        if (n < 0) {
          if (errno == EAGAIN || errno == EWOULDBLOCK)
            break;
          if (errno == EINTR)
            continue;
          _statusOnline.store(false, std::memory_order_release);
          throw exc_qt::ReceiveDataException();
        }
      } // for
    } catch (...) {
      fcntl(_socketFd, F_SETFL, flags);
      throw;
    }

    // Возвращаем исходный режим
    fcntl(_socketFd, F_SETFL, flags);

    // 3. Отправка
    std::size_t totalSent = 0;
    const std::size_t need = packetWithSize.size();

    while (totalSent < need) {
      ssize_t bytesSent = ::send(_socketFd, packetWithSize.data() + totalSent, need - totalSent,
#ifdef MSG_NOSIGNAL
                                 MSG_NOSIGNAL
#else
                                 0
#endif
      );

      if (bytesSent > 0) {
        totalSent += static_cast<std::size_t>(bytesSent);
        continue;
      }

      if (bytesSent < 0) {
        if (errno == EINTR)
          continue;

        if (errno == EAGAIN || errno == EWOULDBLOCK) {
          pollfd pfd{};
          pfd.fd = _socketFd;
          pfd.events = POLLOUT;
          int pr = ::poll(&pfd, 1, 1000);
          if (pr > 0 && (pfd.revents & POLLOUT))
            continue;

          _statusOnline.store(false, std::memory_order_release);
          throw exc_qt::SendDataException();
        }

        if (errno == EPIPE || errno == ECONNRESET) {
          _statusOnline.store(false, std::memory_order_release);
          throw exc_qt::ConnectionToServerException();
        }

        _statusOnline.store(false, std::memory_order_release);
        throw exc_qt::SendDataException();
      };
    }

    // 4. Приём заголовка длины (4 байта)
    len = 0;
    std::vector<PacketDTO> responcePacketListDTOVector;
    try {

      std::uint8_t lenBuf[4];
      std::size_t total = 0;
      while (total < 4) {
        ssize_t r = ::recv(_socketFd, lenBuf + total, 4 - total, 0);

        if (r > 0) {
          total += static_cast<std::size_t>(r);
          continue;
        }

        if (r == 0) {
          _statusOnline.store(false, std::memory_order_release);
          throw exc_qt::ConnectionToServerException();
        }

        if (r < 0) {
          if (errno == EAGAIN || errno == EWOULDBLOCK)
            continue;
          if (errno == EINTR) {
            continue;
          }
          _statusOnline.store(false, std::memory_order_release);
          throw exc_qt::ReceiveDataException();
        }
      }

      std::memcpy(&len, lenBuf, 4);
      len = ntohl(len);
      std::vector<std::uint8_t> buffer(len);

      // 5. Приём тела
      ssize_t bytesReceived = 0;

      while (bytesReceived < len) {
        ssize_t bytes = ::recv(_socketFd, buffer.data() + bytesReceived, len - bytesReceived, 0);

        if (bytes > 0) {
          bytesReceived += bytes;
          continue;
        }

        if (bytes == 0) {
          _statusOnline.store(false, std::memory_order_release);
          throw exc_qt::ConnectionToServerException();
        }

        if (bytes < 0) {
          if (errno == EAGAIN || errno == EWOULDBLOCK)
            continue;
          if (errno == EINTR) {
            continue;
          }
          _statusOnline.store(false, std::memory_order_release);
          throw exc_qt::ReceiveDataException();
        }

        bytesReceived += bytes;
      } // while
      responcePacketListDTOVector = deSerializePacketList(buffer);
    } // try
    catch (...) {
      throw;
    }

    // проверка
    // std::cout << "[DEBUG] buffer.size() = " << buffer.size() << std::endl;
    // for (std::size_t i = 0; i < buffer.size(); ++i)
    //   std::cout << std::hex << static_cast<int>(buffer[i]) << " ";
    // std::cout << std::dec << std::endl;

    // проверка
    // std::cerr << "[DEBUG] Получено от сервера: " << bytesReceived << " байт" << std::endl;
    // for (std::size_t i = 0; i < buffer.size(); ++i) {
    //   std::cerr << static_cast<int>(buffer[i]) << " ";
    // }
    // std::cerr << std::endl;

    for (const auto &pct : responcePacketListDTOVector)
      packetListDTOresult.packets.push_back(pct);

    // for (const auto &packet : packetListDTOresult.packets) {
    //   std::cerr << "[CLIENT DEBUG] Получен пакет requestType = " << static_cast<int>(packet.requestType) <<
    //   std::endl;
    // }

    if (packetListDTOresult.packets.empty())
      throw exc_qt::ReceiveDataException();
  } // try
  catch (const exc_qt::ConnectionToServerException &ex) {
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral("Клиент. Потеряно соединение с сервером."));
    return packetListDTOresult;
  } catch (const exc_qt::SendDataException &ex) {
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()), QStringLiteral("Клиент getDatafromServer: ."));
    packetListDTOresult.packets.clear();
    return packetListDTOresult;
  } catch (const exc_qt::ReceiveDataException &ex) {
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()), QStringLiteral("Клиент getDatafromServer: ."));
    packetListDTOresult.packets.clear();
    return packetListDTOresult;
  }
  return packetListDTOresult;
}
//
//
PacketListDTO ClientSession::processingRequestToServer(std::vector<PacketDTO> &packetDTOListVector,
                                                       const RequestType &requestType) {

  PacketListDTO packetListDTOForSend;
  PacketListDTO packetListDTOresult;
  packetListDTOresult.packets.clear();

  // ГАРД: офлайн/битый сокет → мгновенный отказ без сети
  if (!_statusOnline.load(std::memory_order_acquire) || _socketFd < 0) {
    emit exc_qt::ErrorBus::i().error(QStringLiteral("Нет соединения с сервером"),
                                     QStringLiteral("Клиент processingRequestToServer"));
    return packetListDTOresult; // пустой ответ
  }

  try {
    // добавляем хэдер
    UserLoginPasswordDTO userLoginPasswordDTO;

    if (_instance.getActiveUser())
      userLoginPasswordDTO.login = _instance.getActiveUser()->getLogin();
    else
      userLoginPasswordDTO.login = "!";
    userLoginPasswordDTO.passwordhash = "UserHeder";

    PacketDTO packetDTO;
    packetDTO.requestType = requestType;
    packetDTO.structDTOClassType = StructDTOClassType::userLoginPasswordDTO;
    packetDTO.reqDirection = RequestDirection::ClientToSrv;
    packetDTO.structDTOPtr = std::make_shared<StructDTOClass<UserLoginPasswordDTO>>(userLoginPasswordDTO);

    packetListDTOForSend.packets.push_back(packetDTO);

    for (const auto &packetDTO : packetDTOListVector)
      packetListDTOForSend.packets.push_back(packetDTO);

    // проверка удалить
    // std::cout << "[DEBUG] Сериализуем reqDirection = "
    // << static_cast<int>(packetDTO.reqDirection) << std::endl;

    auto packetListBinarySend = serializePacketList((packetListDTOForSend.packets));

    // отправили и получили ответ от сервера
    packetListDTOresult = getDatafromServer(packetListBinarySend);

  } // try
  catch (const exc_qt::LostConnectionException &ex) {
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral("Клиент processingRequestToServer: "));
    packetListDTOresult.packets.clear();
  }
  return packetListDTOresult;
}

//
//
//
// utilities

bool ClientSession::initServerConnection() {

  this->getInstance().setIsServerStatus(false);

  if (!findServerAddress(getserverConnectionConfigCl(), getserverConnectionModeCl())) {
    getserverConnectionModeCl() = ServerConnectionMode::Offline;
    emit serverStatusChanged(false, getserverConnectionModeCl());
    return false;
  }

  const int fd = createConnection(getserverConnectionConfigCl(), getserverConnectionModeCl());

  if (fd < 0) {
    getserverConnectionModeCl() = ServerConnectionMode::Offline;
    emit serverStatusChanged(false, getserverConnectionModeCl());
    return false;
  }
  _socketFd = fd;
  this->getInstance().setIsServerStatus(true);
  emit serverStatusChanged(true, getserverConnectionModeCl());
  return true;
}

void ClientSession::resetSessionData() {
  _instance = ChatSystem(); // пересоздание всего chatSystem (users, chats, id)
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
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "Клиент: регистрация на устройстве. Неверный тип пакета с сервера."));
    return false;
  } catch (const exc_qt::NetworkException &ex) {
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral("Клиент: регистрация на устройстве. Неизвестная ошибка."));
    return false;
  }

  return true;
}

//
//
//

bool ClientSession::createUserCl(std::shared_ptr<User> &user) {

  UserDTO userDTO;

  userDTO.userName = user->getUserName();
  userDTO.login = user->getLogin();
  userDTO.passwordhash = user->getPasswordHash();
  userDTO.email = user->getEmail();
  userDTO.phone = user->getPhone();

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

  try {

    const auto &packet = static_cast<const StructDTOClass<ResponceDTO> &>(*packetListDTOresult.packets[0].structDTOPtr)
                             .getStructDTOClass();

    if (packet.reqResult)
      this->_instance.addUserToSystem(user);
    else
      throw exc_qt::CreateUserException();
  } catch (const exc_qt::NetworkException &ex) {
    std::cerr << "Клиент: " << ex.what() << std::endl;
    return false;
  }
  return true;
}
//
//
//
bool ClientSession::createNewChatCl(std::shared_ptr<Chat> &chat) {

  // логика такая
  // формируем чат и сообщение в пакет и отправляем на сервер
  // в ответ мы получаем номера чата и сообщения
  // если все ок, то вводим чат и сообщение в систему

  // формируем пакет для отправки чата
  auto chatDTO = FillForSendOneChatDTOFromClient(chat);

  // формируем пакет для отправки сообщений
  auto messageChatDTO = fillChatMessageDTOFromClient(chat);

  PacketDTO chatPacket;
  chatPacket.requestType = RequestType::RqFrClientCreateChat;
  chatPacket.structDTOClassType = StructDTOClassType::chatDTO;
  chatPacket.reqDirection = RequestDirection::ClientToSrv;
  chatPacket.structDTOPtr = std::make_shared<StructDTOClass<ChatDTO>>(chatDTO.value());

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
          //   std::cout << "[DEBUG] anyNumber = '" << packetDTO.anyNumber << "'" << std::endl;
          auto generalChatId = packetDTO.anyNumber;
          chat->setChatId(generalChatId);

          //   std::cout << "[DEBUG] anyString = '" << packetDTO.anyString << "'" << std::endl;

          auto generalMessageId = parseGetlineToSizeT(packetDTO.anyString);
          chat->getMessages().begin()->second->setMessageId(generalMessageId);

          if (chat->getMessages().empty())
            throw exc_qt::CreateMessageException();

          // добавляем чат в систему
          _instance.addChatToInstance(chat);
        }
      }
    }
  } // try
  catch (const exc_qt::WrongresponceTypeException &ex) {
    std::cout << "Клиент. createNewChatCl. " << ex.what() << std::endl;
    return false;
  } catch (const exc_qt::EmptyPacketException &ex) {
    std::cout << "Клиент. createNewChatCl. " << ex.what() << std::endl;
    return false;
  } catch (const exc_qt::CreateChatException &ex) {
    std::cout << "Клиент. createNewChatCl. " << ex.what() << std::endl;
    return false;
  } catch (const exc_qt::CreateChatIdException &ex) {
    std::cout << "Клиент. createNewChatCl. " << ex.what() << std::endl;
    return false;
  } catch (const exc_qt::CreateMessageIdException &ex) {
    std::cout << "Клиент. createNewChatCl. " << ex.what() << std::endl;
    return false;
  } catch (const exc_qt::CreateMessageException &ex) {
    std::cout << "Клиент. createNewChatCl. " << ex.what() << std::endl;
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
bool ClientSession::createMessageCl(const Message &message, std::shared_ptr<Chat> &chat_ptr,
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
      auto newMessageId = parseGetlineToSizeT(packetDTO.anyString);

      if (chat_ptr->getMessages().empty())
        throw exc_qt::CreateMessageException();

      message_ptr->setMessageId(newMessageId);

      chat_ptr->addMessageToChat(message_ptr, user, false);
    }
  } catch (const exc_qt::EmptyPacketException &ex) {
    std::cerr << "Клиент: createMessageCl" << ex.what() << std::endl;
    return false;
  } catch (const exc_qt::WrongPacketSizeException &ex) {
    std::cerr << "Клиент: createMessageCl" << ex.what() << std::endl;
    return false;
  } catch (const exc_qt::WrongresponceTypeException &ex) {
    std::cerr << "Клиент: createMessageCl" << ex.what() << std::endl;
    return false;

  } catch (const exc_qt::CreateMessageException &ex) {
    std::cerr << "Клиент: createMessageCl" << ex.what() << std::endl;
    return false;
  } catch (const exc_qt::ValidationException &ex) {
    std::cerr << "Клиент: createMessageCl" << ex.what() << std::endl;
    return false;
  }
  return true;
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
    std::cout << "Клиент. sendLastReadMessageFromClient. " << ex.what() << std::endl;
    return false;
  } catch (const exc_qt::EmptyPacketException &ex) {
    std::cout << "Клиент. sendLastReadMessageFromClient. " << ex.what() << std::endl;
    return false;
  } catch (const exc_qt::LastReadMessageException &ex) {
    std::cout << "Клиент. sendLastReadMessageFromClient. " << ex.what() << std::endl;
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
  //   std::cout << "В систему добавлен Пользователь Имя / Логин " << user_ptr->getUserName() << " / "
  //             << user_ptr->getLogin() << std::endl;
}
//
//
// получение одного сообщения
void ClientSession::setOneMessageDTO(const MessageDTO &messageDTO, const std::shared_ptr<Chat> &chat) const {

  // получить указатель на пользователя - отправителя сообщения
  auto sender = this->_instance.findUserByLogin(messageDTO.senderLogin);

  // упрощенная передача только одного текстового сообщения

  if (messageDTO.messageContent.empty())
    throw exc_qt::UnknownException("DTO-сообщение не содержит содержимого");

  auto message = createOneMessage(messageDTO.messageContent[0].payload, sender, messageDTO.timeStamp,
                                  messageDTO.messageId);

  chat->addMessageToChat(std::make_shared<Message>(message), sender, false);

  //   std::cout << "В систему добавлено сообщение MessageId " << message.getMessageId() << " в чат chatId "
  //             << chat->getChatId() << std::endl;

  // ⬇⬇⬇ Вставка проверки
  // std::cout << "[DEBUG] Добавлено сообщение:\n";
  // std::cout << "- msgId: " << message.getMessageId();

  // if (sender) {
  //   std::cout << ", sender: " << sender->getLogin();
  // } else {
  //   std::cout << ", sender: [unknown]";
  // }

  // std::cout << ", timeStamp: "
  //           << formatTimeStampToString(message.getTimeStamp(), true)
  //           << ", content: ";

  // for (const auto &content_ptr : message.getContent()) {
  //   auto textContent_ptr =
  //       std::dynamic_pointer_cast<MessageContent<TextContent>>(content_ptr);
  //   if (textContent_ptr) {
  //     std::cout << textContent_ptr->getMessageContent()._text << " ";
  //   } else {
  //     std::cout << "[non-text content] ";
  //   }
  // }

  // std::cout << std::endl;
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

      //   // ⬇⬇⬇ Вставка проверки
      //   const auto &messages = chat_ptr->getMessages();

      //   std::cout << "\n[DEBUG] Чат " << chat_ptr->getChatId()
      //             << ", всего сообщений: " << messages.size() << std::endl;

      //   for (const auto &[timeStamp, message_ptr] : messages) {
      //     std::cout << "- msgId: " << message_ptr->getMessageId();

      //     auto sender_ptr = message_ptr->getSender().lock();
      //     if (sender_ptr) {
      //       std::cout << ", sender: " << sender_ptr->getLogin();
      //     } else {
      //       std::cout << ", sender: [unknown]";
      //     }

      //     std::cout << ", timeStamp: " << formatTimeStampToString(timeStamp,
      //     true)
      //               << ", content: ";

      //     for (const auto &content_ptr : message_ptr->getContent()) {
      //       auto textContent_ptr =
      //           std::dynamic_pointer_cast<MessageContent<TextContent>>(
      //               content_ptr);
      //       if (textContent_ptr) {
      //         std::cout << textContent_ptr->getMessageContent()._text << " ";
      //       } else {
      //         std::cout << "[non-text content] ";
      //       }
      //     }

      //     std::cout << std::endl;
      //   }
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
      // доделать очередь

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
          std::cout << "В систему добавлен участник чата. Имя/Логин: " << newUser_ptr->getUserName() << " / "
                    << newUser_ptr->getLogin() << std::endl;
        }
      }
    } else { // если не пришел ответ с сервера
      std::cerr << "Ошибка. Не сомгли получить пользователей с сервера. Будут "
                   "временно добавлены в систему только логины."
                << std::endl;

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

  std::cout << "В систему добавлен чат. ChatId: " << chat_ptr->getChatId() << std::endl;

  // ⬇⬇⬇ Проверка
  // std::cout << "\n[DEBUG] Добавлен чат в клиента:\n";
  // std::cout << "chatId = " << chat_ptr->getChatId() << std::endl;
  // std::cout << "lastMessageTime = "
  //           << formatTimeStampToString(
  //                  chat_ptr->getTimeStampForLastMessage(
  //                      chat_ptr->getLastReadMessageId(
  //                          this->_instance.getActiveUser())),
  //                  true)

  //           << std::endl;

  // std::cout << "участники:" << std::endl;
  // for (const auto &participant : chat_ptr->getParticipants()) {
  //   auto user = participant._user.lock();
  //   if (user) {
  //     std::cout << "- " << user->getLogin()
  //               << " | deleted: " << participant._deletedFromChat <<
  //               std::endl;
  //   } else {
  //     std::cout << "- удалённый пользователь" << std::endl;
  //   }
}
std::optional<ChatDTO> ClientSession::FillForSendOneChatDTOFromClient(const std::shared_ptr<Chat> &chat_ptr) {
  ChatDTO chatDTO;

  // взяли chatId
  chatDTO.chatId = chat_ptr->getChatId();

  if (!_instance.getActiveUser())
    return std::nullopt;
  chatDTO.senderLogin = _instance.getActiveUser()->getLogin();

  try {
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

        participantsDTO.deletedFromChat = chat_ptr->getUserDeletedFromChat(user_ptr);

        chatDTO.participants.push_back(participantsDTO);

      } // if user_ptr
      else
        // throw exc_qt::UserNotFoundException();
        continue;
    } // for participants
  }
  // try
  catch (const exc_qt::UserNotFoundException &ex) {
    // std::cerr << "Клиент: FillForSendOneChatDTOFromClient. " << ex.what() << std::endl;
    // return std::nullopt;
  }
  if (chatDTO.participants.empty())
    return std::nullopt;
  return chatDTO;
}
//
//
// получаем одно конкретное сообщение пользователя
MessageDTO ClientSession::FillForSendOneMessageDTOFromClient(const std::shared_ptr<Message> &message,
                                                             const std::size_t &chatId) {
  MessageDTO messageDTO;
  auto user_ptr = message->getSender().lock();

  messageDTO.senderLogin = user_ptr ? user_ptr->getLogin() : "";

  messageDTO.chatId = chatId;
  messageDTO.messageId = message->getMessageId();
  messageDTO.timeStamp = message->getTimeStamp();

  // получаем контент
  MessageContentDTO temContent;
  temContent.messageContentType = MessageContentType::Text;
  auto contentElement = message->getContent().front();

  auto contentTextPtr = std::dynamic_pointer_cast<MessageContent<TextContent>>(contentElement);

  if (contentTextPtr) {
    auto contentText = contentTextPtr->getMessageContent();
    temContent.payload = contentText._text;
  }

  messageDTO.messageContent.push_back(temContent);

  return messageDTO;
}

// передаем сообщения пользователя конкретного чата
MessageChatDTO ClientSession::fillChatMessageDTOFromClient(const std::shared_ptr<Chat> &chat) {
  MessageChatDTO messageChatDTO;

  // взяли chatId
  messageChatDTO.chatId = chat->getChatId();

  for (const auto &message : chat->getMessages()) {

    messageChatDTO.messageDTO.push_back(FillForSendOneMessageDTOFromClient(message.second, messageChatDTO.chatId));

  } // for message
  return messageChatDTO;
}
