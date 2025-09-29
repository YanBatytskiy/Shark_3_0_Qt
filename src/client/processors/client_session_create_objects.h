#pragma once

#include <cstddef>
#include <memory>

class ClientSession;
class ClientRequestExecutor;
class ClientSessionDtoBuilder;
class Chat;
class Message;
class User;
struct ChatDTO;
struct MessageChatDTO;
struct MessageDTO;
struct UserDTO;

class ClientSessionCreateObjects {
 public:
  ClientSessionCreateObjects(ClientSession &session,
                             ClientRequestExecutor &request_executor,
                             ClientSessionDtoBuilder &dto_builder);

  bool createUserProcessing(const UserDTO &user_dto);
  bool createNewChatProcessing(std::shared_ptr<Chat> &chat, ChatDTO &chat_dto,
                               MessageChatDTO &message_chat_dto);
  std::size_t createMessageProcessing(const Message &message,
                                      std::shared_ptr<Chat> &chat_ptr,
                                      const std::shared_ptr<User> &user);

 private:
  ClientSession &session_;
  ClientRequestExecutor &request_executor_;
  ClientSessionDtoBuilder &dto_builder_;
};
