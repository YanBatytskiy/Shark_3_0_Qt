#pragma once

#include <cstddef>
#include <memory>

class ClientSession;
class Chat;
class Message;
class User;
struct ChatDTO;
struct MessageChatDTO;
struct MessageDTO;
struct UserDTO;

class ClientSessionCreateObjects {
public:
  explicit ClientSessionCreateObjects(ClientSession &session);

  bool createUserCl(const UserDTO &userDTO);
  bool createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chatDTO, MessageChatDTO &messageChatDTO);
  std::size_t createMessageCl(const Message &message, std::shared_ptr<Chat> &chat_ptr, const std::shared_ptr<User> &user);

private:
  ClientSession &_session;
};
