#pragma once
#include "chat_system/chat_system.h"
#include "dto_struct.h"
#include "message/message.h"
#include <cstdint>
#include <memory>
#include <optional>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <QThread>
#include <qthread.h>
#include <qtmetamacros.h>

class ConnectionMonitor;

enum class ServerConnectionMode {
  Localhost,    // на этом же компьютере
  LocalNetwork, // внутри LAN (UDP discovery)
  Offline       //
};

struct ServerConnectionConfig {
  std::string addressLocalHost = "127.0.0.1";
  std::string addressLocalNetwork = "";
  std::uint16_t port = 50000;
  bool found = false;
};

class ClientSession : public QObject {
  Q_OBJECT
private:
  ChatSystem &_instance; // link to server
  int _socketFd;
  ServerConnectionConfig _serverConnectionConfig;
  ServerConnectionMode _serverConnectionMode;

  QThread _netThread;
  ConnectionMonitor *_connectionMonitor{nullptr};
  std::atomic_bool _statusOnline{false};

signals:
  void serverStatusChanged(bool online, ServerConnectionMode mode);

public slots:
  void onConnectionStateChanged(bool online, ServerConnectionMode mode);

public:
  bool getIsServerOnline() const noexcept { return _statusOnline.load(std::memory_order_acquire); }

  // constructors
  ClientSession(ChatSystem &client, QObject *parent = nullptr);
  ClientSession(const ClientSession &) = delete;
  ClientSession &operator=(const ClientSession &) = delete;
  ClientSession(ClientSession &&) = delete;
  ClientSession &operator=(ClientSession &&) = delete;

  ~ClientSession() {
    stopConnectionThread(); // quit(); wait();
  }

  // qt methods
  //  utilities

  bool reInitilizeBaseQt();

  bool checkLoginQt(std::string login);

  bool checkLoginPsswordQt(std::string login, std::string password);

  bool registerClientOnDeviceQt(std::string login);

  bool inputNewLoginValidationQt(std::string inputData);

  bool inputNewPasswordValidationQt(std::string inputData, std::size_t dataLengthMin, std::size_t dataLengthMax);

  std::optional<std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>>> getChatListQt();

  bool CreateAndSendNewChatQt(std::shared_ptr<Chat> &chat_ptr, std::vector<UserDTO> &participants, Message &newMessage);

    bool createUserQt(UserDTO userDTO);

    bool changeUserDataQt(UserDTO userDTO);

    bool changeUserPasswordQt(UserDTO userDTO);

    bool blockUnblockUserQt(std::string login, bool isBlocked, std::string disableReason);

    bool bunUnbunUserQt(std::string login, bool isBanned, std::int64_t bunnedTo);


  // threads

  void startConnectionThread();
  void stopConnectionThread();

  // getters

  ConnectionMonitor *getConnectionMonitor();

  ServerConnectionConfig &getserverConnectionConfigCl();

  const ServerConnectionConfig &getserverConnectionConfigCl() const;

  ServerConnectionMode &getserverConnectionModeCl();

  const ServerConnectionMode &getserverConnectionModeCl() const;

  const std::shared_ptr<User> getActiveUserCl() const;

  ChatSystem &getInstance();

  std::size_t getSocketFd() const;

  // setters

  void setActiveUserCl(const std::shared_ptr<User> &user);

  void setSocketFd(int socketFd);

  // checking and finding

  // поиск и проверки пользователя
  const std::vector<UserDTO> findUserByTextPartOnServerCl(const std::string &textToFind);

  bool checkUserLoginCl(const std::string &userLogin);

  bool checkUserPasswordCl(const std::string &userLogin, const std::string &passwordHash);

  // transport

  //   void reidentifyClientAfterConnection();

  bool findServerAddress(ServerConnectionConfig &serverConnectionConfig, ServerConnectionMode &serverConnectionMode);

  int createConnection(ServerConnectionConfig &serverConnectionConfig, ServerConnectionMode &serverConnectionMode);

  bool discoverServerOnLAN(ServerConnectionConfig &serverConnectionConfig);

  PacketListDTO getDatafromServer(const std::vector<std::uint8_t> &packetListSend);

  PacketListDTO processingRequestToServer(std::vector<PacketDTO> &packetDTOVector, const RequestType &requestType);

  //
  //
  // utilities

  bool initServerConnection();

  void resetSessionData();

  bool reInitilizeBaseCl();

  const std::size_t createNewMessageIdFromCl() const;

  bool registerClientToSystemCl(const std::string &login);

  bool changeUserDataCl(const UserDTO& userDTO);

  bool createUserCl(const UserDTO& userDTO);

  bool createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chatDTO, MessageChatDTO &messageChatDTO);

  std::size_t createMessageCl(const Message &message, std::shared_ptr<Chat> &chat, const std::shared_ptr<User> &user);

  // отправить на сервер lastReadMessage
  bool sendLastReadMessageFromClient(const std::shared_ptr<Chat> &chat_ptr, std::size_t messageId);

  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // transport setters

  // установка пользователя
  void setActiveUserDTOFromSrv(const UserDTO &userDTO) const;

  void setUserDTOFromSrv(const UserDTO &userDTO) const;

  //   установка одного сообщения
  void setOneMessageDTO(const MessageDTO &messageDTO, const std::shared_ptr<Chat> &chat) const;

  // установка сообщений одного чата
  bool setOneChatMessageDTO(const MessageChatDTO &messageChatDTO) const;

  bool checkAndAddParticipantToSystem(const std::vector<std::string> &participants);

  // установка чат лист пльзователя

  void setOneChatDTOFromSrv(const ChatDTO &chatDTO);

  MessageDTO fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chatId);

  // заполнение на отправку пакета Chat
  std::optional<ChatDTO> fillChatDTOQt(const std::shared_ptr<Chat> &chat);

  // MessageDTO FillForSendOneMessageDTOFromClient(const std::shared_ptr<Message> &message, const std::size_t &chatId);

  // // передаем сообщения пользователя конкретного чата
  // MessageChatDTO fillChatMessageDTOFromClient(const std::shared_ptr<Chat> &chat);
};