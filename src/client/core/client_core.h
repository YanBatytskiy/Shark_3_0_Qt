#pragma once

#include <QObject>
#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "client/request_dispatcher/request_dispatcher.h"
#include "client/tcp_transport/session_types.h"
#include "client/tcp_transport/tcp_transport.h"
#include "dto_struct.h"

class ChatSystem;
class Chat;
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

  int getSocketFdCore() const;

  void setSocketFdCore(int socket_fd);

  const std::vector<UserDTO> findUserByTextPartOnServerCore(
      const std::string &text_to_find);

  bool checkUserLoginCore(const std::string &user_login);

  bool checkUserPasswordCore(const std::string &user_login,
                             const std::string &password_plain);

  bool findServerAddressCore(ServerConnectionConfig &config,
                             ServerConnectionMode &mode);

  int createConnectionCore(const ServerConnectionConfig &config,
                           ServerConnectionMode mode);

  bool discoverServerOnLANCore(ServerConnectionConfig &config);

  PacketListDTO getDatafromServerCore(const std::vector<std::uint8_t> &payload);

  PacketListDTO processingRequestToServerCore(std::vector<PacketDTO> &packets,
                                              const RequestType &request_type);

  bool initServerConnectionCore();

  void resetSessionDataCore();

  bool reInitilizeBaseCore();

  bool sendLastReadMessageFromClientCore(const std::shared_ptr<Chat> &chat_ptr,
                                         std::size_t message_id);

  bool ensureParticipantsAvailableCore(
      const std::vector<std::string> &participants);

  bool blockUnblockUserCore(const std::string &login, bool is_blocked,
                            const std::string &disable_reason);

  bool bunUnbunUserCore(const std::string &login, bool is_banned,
                        std::int64_t banned_to);

  bool changeUserPasswordCore(const UserDTO &user_dto);

 signals:
  void serverStatusChanged(bool online, ServerConnectionMode mode);

 private:
  void updateConnectionStateCore(bool online, ServerConnectionMode mode);

  ChatSystem &chat_system_;
  TcpTransport transport_;
  RequestDispatcher request_dispatcher_;

  std::atomic_bool status_online_{false};
  ServerConnectionConfig server_connection_config_{};
  ServerConnectionMode server_connection_mode_{ServerConnectionMode::Offline};

  mutable std::mutex socket_mutex_;
  int socket_fd_{-1};
};
