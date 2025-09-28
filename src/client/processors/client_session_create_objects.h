#pragma once

#include <cstddef>
#include <memory>

class ClientCore;
class Chat;
class Message;
class User;
struct ChatDTO;
struct MessageChatDTO;
struct MessageDTO;
struct UserDTO;

class ClientSessionCreateObjects {
public:
  explicit ClientSessionCreateObjects(ClientCore &core);

  bool createUserCl(const UserDTO &user_dto);
  bool createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chat_dto, MessageChatDTO &message_chat_dto);
  std::size_t createMessageCl(const Message &message, std::shared_ptr<Chat> &chat_ptr,
                              const std::shared_ptr<User> &user);

private:
  ClientCore &core_;
};
