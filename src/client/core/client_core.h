#pragma once

#include "client/processors/client_session_create_objects.h"
#include "client/processors/client_session_dto_builder.h"
#include "client/processors/client_session_dto_writer.h"
#include "client/processors/client_session_modify_objects.h"
#include "client/request_dispatcher/request_dispatcher.h"
#include "client/tcp_transport/session_types.h"
#include "client/tcp_transport/tcp_transport.h"
#include "dto_struct.h"

#include <QObject>

#include <atomic>
#include <map>
#include <multimap>
#include <string>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

class ChatSystem;
class Chat;
class Message;
class User;

class ClientCore : public QObject {
  Q_OBJECT
public:
  ClientCore(ChatSystem &chat_system, QObject *parent = nullptr);
  ClientCore(const ClientCore &) = delete;
  ClientCore &operator=(const ClientCore &) = delete;
  ClientCore(ClientCore &&) = delete;
  ClientCore &operator=(ClientCore &&) = delete;

  ~ClientCore();

  bool getIsServerOnlineCore() const noexcept;

  ServerConnectionConfig &getServerConnectionConfigCore();
  const ServerConnectionConfig &getServerConnectionConfigCore() const;

  ServerConnectionMode &getServerConnectionModeCore();
  const ServerConnectionMode &getServerConnectionModeCore() const;

  ChatSystem &getInstance();
  const std::shared_ptr<User> getActiveUserCl() const;

  int getSocketFdCore() const;

  void setActiveUserCl(const std::shared_ptr<User> &user);
  void setSocketFdCore(int socket_fd);

  std::optional<std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>>> getChatListCl();
  const std::vector<UserDTO> findUserByTextPartOnServerCl(const std::string &text_to_find);
  bool checkUserLoginCl(const std::string &user_login);
  bool checkUserPasswordCl(const std::string &user_login, const std::string &password_plain);

  bool findServerAddressCore(ServerConnectionConfig &config, ServerConnectionMode &mode);
  int createConnectionCore(const ServerConnectionConfig &config, ServerConnectionMode mode);
  bool discoverServerOnLANCore(ServerConnectionConfig &config);
  PacketListDTO getDatafromServerCore(const std::vector<std::uint8_t> &payload);
  PacketListDTO processingRequestToServerCore(std::vector<PacketDTO> &packets, const RequestType &request_type);

  bool initServerConnectionCore();
  void resetSessionDataCore();
 
  bool reInitilizeBaseCl();
  bool registerClientToSystemCl(const std::string &login);
  bool changeUserDataCl(const UserDTO &user_dto);
  bool createUserCl(const UserDTO &user_dto);
  bool CreateAndSendNewChatCl(std::shared_ptr<Chat> &chat_ptr, std::vector<UserDTO> &participants, Message &new_message);
  bool createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chat_dto, MessageChatDTO &message_chat_dto);
  MessageDTO fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chat_id);
  std::size_t createMessageCl(const Message &message, std::shared_ptr<Chat> &chat_ptr, const std::shared_ptr<User> &user);
  bool sendLastReadMessageFromClient(const std::shared_ptr<Chat> &chat_ptr, std::size_t message_id);
  bool ensureParticipantsAvailable(const std::vector<std::string> &participants);
  std::optional<ChatDTO> fillChatDTOCl(const std::shared_ptr<Chat> &chat_ptr);
  bool blockUnblockUserCl(const std::string &login, bool is_blocked, const std::string &disable_reason);
  bool bunUnbunUserCl(const std::string &login, bool is_banned, std::int64_t banned_to);
  bool changeUserPasswordCl(const UserDTO &user_dto);

signals:
  void serverStatusChanged(bool online, ServerConnectionMode mode);

private:
  void updateConnectionState(bool online, ServerConnectionMode mode);

  ChatSystem &chat_system_;
  TcpTransport transport_;
  RequestDispatcher request_dispatcher_;
  ClientSessionDtoWriter dto_writer_;
  ClientSessionDtoBuilder dto_builder_;
  ClientSessionCreateObjects create_objects_;
  ClientSessionModifyObjects modify_objects_;

  std::atomic_bool status_online_{false};
  ServerConnectionConfig server_connection_config_{};
  ServerConnectionMode server_connection_mode_{ServerConnectionMode::Offline};

  mutable std::mutex socket_mutex_;
  int socket_fd_{-1};
};
