#pragma once

#include <cstddef>
#include <memory>
#include <optional>

class ClientSession;
class Chat;
class Message;
class User;
struct ChatDTO;
struct MessageChatDTO;
struct MessageDTO;
struct UserDTO;

class ClientSessionDtoFromCl {
public:
  explicit ClientSessionDtoFromCl(ClientSession &session);

  MessageDTO fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chatId);
  std::optional<ChatDTO> fillChatDTOQt(const std::shared_ptr<Chat> &chat);

  bool changeUserDataCl(const UserDTO &userDTO);
  bool createUserCl(const UserDTO &userDTO);
  bool createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chatDTO, MessageChatDTO &messageChatDTO);
  std::size_t createMessageCl(const Message &message, std::shared_ptr<Chat> &chat_ptr, const std::shared_ptr<User> &user);
  bool sendLastReadMessageFromClient(const std::shared_ptr<Chat> &chat_ptr, std::size_t messageId);

private:
  ClientSession &_session;
};
