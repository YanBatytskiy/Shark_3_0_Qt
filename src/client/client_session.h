#pragma once

#include "client/tcp_transport/session_types.h"
#include "dto_struct.h"
#include "message/message.h"

#include <QObject>

#include <atomic>
#include <memory>
#include <optional>
#include <thread>
#include <vector>
#include <map>

class ChatSystem;
class Chat;
class User;
class ClientCore;

class ClientSession : public QObject {
  Q_OBJECT
public:
  explicit ClientSession(ChatSystem &chat_system, QObject *parent = nullptr);
  ClientSession(const ClientSession &) = delete;
  ClientSession &operator=(const ClientSession &) = delete;
  ClientSession(ClientSession &&) = delete;
  ClientSession &operator=(ClientSession &&) = delete;

  ~ClientSession();

  bool getIsServerOnline() const noexcept;

  bool inputNewLoginValidationQt(std::string input_data);
  bool inputNewPasswordValidationQt(std::string input_data, std::size_t data_length_min, std::size_t data_length_max);
  std::optional<std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>>> getChatListCl();
  bool CreateAndSendNewChatCl(std::shared_ptr<Chat> &chat_ptr, std::vector<UserDTO> &participants, Message &new_message);
  bool changeUserPasswordCl(UserDTO user_dto);
  bool blockUnblockUserCl(const std::string &login, bool is_blocked, const std::string &disable_reason);
  bool bunUnbunUserCl(const std::string &login, bool is_banned, std::int64_t banned_to);

  void startConnectionThread();
  void stopConnectionThread();

  ServerConnectionConfig &getserverConnectionConfigCl();
  const ServerConnectionConfig &getserverConnectionConfigCl() const;
  ServerConnectionMode &getserverConnectionModeCl();
  const ServerConnectionMode &getserverConnectionModeCl() const;

  const std::shared_ptr<User> getActiveUserCl() const;
  ChatSystem &getInstance();
  std::size_t getSocketFd() const;

  void setActiveUserCl(const std::shared_ptr<User> &user);
  void setSocketFd(int socket_fd);

  const std::vector<UserDTO> findUserByTextPartOnServerCl(const std::string &text_to_find);
  bool checkUserLoginCl(const std::string &user_login);
  bool checkUserPasswordCl(const std::string &user_login, const std::string &password);

  bool findServerAddress(ServerConnectionConfig &server_connection_config, ServerConnectionMode &server_connection_mode);
  int createConnection(ServerConnectionConfig &server_connection_config, ServerConnectionMode &server_connection_mode);
  bool discoverServerOnLAN(ServerConnectionConfig &server_connection_config);
  PacketListDTO getDatafromServer(const std::vector<std::uint8_t> &packet_list_send);
  PacketListDTO processingRequestToServer(std::vector<PacketDTO> &packet_dto_vector, const RequestType &request_type);

  bool initServerConnection();
  void resetSessionData();
  bool reInitilizeBaseCl();

  bool registerClientToSystemCl(const std::string &login);
  bool changeUserDataCl(const UserDTO &user_dto);
  bool createUserCl(const UserDTO &user_dto);
  bool createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chat_dto, MessageChatDTO &message_chat_dto);
  std::size_t createMessageCl(const Message &message, std::shared_ptr<Chat> &chat, const std::shared_ptr<User> &user);
  bool sendLastReadMessageFromClient(const std::shared_ptr<Chat> &chat_ptr, std::size_t message_id);
  bool checkAndAddParticipantToSystem(const std::vector<std::string> &participants);
  MessageDTO fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chat_id);
  std::optional<ChatDTO> fillChatDTOCl(const std::shared_ptr<Chat> &chat);

  std::shared_ptr<ClientCore> getClientCore() const;

signals:
  void serverStatusChanged(bool online, ServerConnectionMode mode);

private:
  void connectionMonitorLoop();
  static bool socketAlive(int fd);

  ChatSystem &chat_system_;
  std::shared_ptr<ClientCore> client_core_;
  std::atomic_bool connection_thread_running_{false};
  std::thread connection_thread_;
};
