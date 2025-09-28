#pragma once

#include <QObject>
#include <atomic>
#include <map>
#include <memory>
#include <optional>
#include <thread>
#include <vector>

#include "client/core/client_core.h"
#include "client/processors/client_session_create_objects.h"
#include "client/processors/client_session_dto_builder.h"
#include "client/processors/client_session_dto_writer.h"
#include "client/processors/client_session_modify_objects.h"
#include "client/tcp_transport/session_types.h"
#include "dto_struct.h"
#include "message/message.h"

class ChatSystem;
class Chat;
class User;

class ClientSession : public QObject {
  Q_OBJECT
 public:
  explicit ClientSession(ChatSystem &chat_system, QObject *parent = nullptr);
  ClientSession(const ClientSession &) = delete;
  ClientSession &operator=(const ClientSession &) = delete;
  ClientSession(ClientSession &&) = delete;
  ClientSession &operator=(ClientSession &&) = delete;

  ~ClientSession();

  bool getIsServerOnlineCl() const noexcept;

  bool inputNewLoginValidationQtCl(std::string input_data);
  bool inputNewPasswordValidationQtCl(std::string input_data,
                                      std::size_t data_length_min,
                                      std::size_t data_length_max);
  std::optional<
      std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>>>
  getChatListCl();
  bool CreateAndSendNewChatCl(std::shared_ptr<Chat> &chat_ptr,
                              std::vector<UserDTO> &participants,
                              Message &new_message);
  bool changeUserPasswordCl(UserDTO user_dto);
  bool blockUnblockUserCl(const std::string &login, bool is_blocked,
                          const std::string &disable_reason);
  bool bunUnbunUserCl(const std::string &login, bool is_banned,
                      std::int64_t banned_to);

  void startConnectionThreadCl();
  void stopConnectionThreadCl();

  ServerConnectionConfig &getserverConnectionConfigCl();
  const ServerConnectionConfig &getserverConnectionConfigCl() const;
  ServerConnectionMode &getserverConnectionModeCl();
  const ServerConnectionMode &getserverConnectionModeCl() const;

  const std::shared_ptr<User> getActiveUserCl() const;
  ChatSystem &getInstanceCl();
  std::size_t getSocketFdCl() const;

  void setActiveUserCl(const std::shared_ptr<User> &user);
  void setSocketFdCl(int socket_fd);

  const std::vector<UserDTO> findUserByTextPartOnServerCl(
      const std::string &text_to_find);
  bool checkUserLoginCl(const std::string &user_login);
  bool checkUserPasswordCl(const std::string &user_login,
                           const std::string &password);

  PacketListDTO processingRequestToServerCl(
      std::vector<PacketDTO> &packet_dto_vector,
      const RequestType &request_type);

  void resetSessionDataCl();
  bool reInitilizeBaseCl();

  bool registerClientToSystemCl(const std::string &login);
  bool changeUserDataCl(const UserDTO &user_dto);
  bool createUserCl(const UserDTO &user_dto);
  bool createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chat_dto,
                       MessageChatDTO &message_chat_dto);
  std::size_t createMessageCl(const Message &message,
                              std::shared_ptr<Chat> &chat,
                              const std::shared_ptr<User> &user);
  bool sendLastReadMessageFromClientCl(const std::shared_ptr<Chat> &chat_ptr,
                                       std::size_t message_id);
  bool checkAndAddParticipantToSystemCl(
      const std::vector<std::string> &participants);
  MessageDTO fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message,
                                     std::size_t chat_id);
  std::optional<ChatDTO> fillChatDTOCl(const std::shared_ptr<Chat> &chat);

 signals:
  void serverStatusChanged(bool online, ServerConnectionMode mode);

 private:
  void connectionMonitorLoopCl();
  static bool socketAliveCl(int fd);

  ChatSystem &_instance;
  ClientCore _core;
  ClientSessionDtoWriter _dtoWriter;
  ClientSessionDtoBuilder _dtoBuilder;
  ClientSessionCreateObjects _createObjects;
  ClientSessionModifyObjects _modifyObjects;
  std::atomic_bool connection_thread_running_{false};
  std::thread connection_thread_;
};
