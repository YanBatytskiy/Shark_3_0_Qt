#pragma once
#include "client_core.h"
#include "tcp_transport/session_types.h"
#include "client/processors/client_session_dto_builder.h"
#include "client/processors/client_session_create_objects.h"
#include "client/processors/client_session_dto_writer.h"
#include "client/processors/client_session_modify_objects.h"
#include "chat_system/chat_system.h"
#include "dto_struct.h"
#include "message/message.h"

#include <map>
#include <memory>
#include <optional>
#include <vector>

class ClientSession : public QObject {
  Q_OBJECT
private:
  ChatSystem &_instance; // link to server
  ClientCore _core;
  ClientSessionDtoWriter _dtoWriter;
  ClientSessionDtoBuilder _dtoBuilder;
  ClientSessionCreateObjects _createObjects;
  ClientSessionModifyObjects _modifyObjects;

signals:
  void serverStatusChanged(bool online, ServerConnectionMode mode);

public:
  bool getIsServerOnline() const noexcept { return _core.getIsServerOnlineCore(); }

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

  bool inputNewLoginValidationQt(std::string inputData);

  bool inputNewPasswordValidationQt(std::string inputData, std::size_t dataLengthMin, std::size_t dataLengthMax);

  std::optional<std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>>> getChatListQt();

  bool CreateAndSendNewChatQt(std::shared_ptr<Chat> &chat_ptr, std::vector<UserDTO> &participants, Message &newMessage);

    bool changeUserPasswordQt(UserDTO userDTO);
    bool blockUnblockUserQt(std::string login, bool isBlocked, std::string disableReason);

    bool bunUnbunUserQt(std::string login, bool isBanned, std::int64_t bunnedTo);


  // threads

  void startConnectionThread();
  void stopConnectionThread();

  // getters

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

  bool checkAndAddParticipantToSystem(const std::vector<std::string> &participants);

  MessageDTO fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chatId);

  // заполнение на отправку пакета Chat
  std::optional<ChatDTO> fillChatDTOQt(const std::shared_ptr<Chat> &chat);
};
