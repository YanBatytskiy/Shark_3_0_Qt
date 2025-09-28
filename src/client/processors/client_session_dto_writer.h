#pragma once

#include <memory>

class ClientCore;
class Chat;
struct ChatDTO;
struct MessageChatDTO;
struct MessageDTO;
struct UserDTO;

class ClientSessionDtoWriter {
public:
  explicit ClientSessionDtoWriter(ClientCore &core);

  void setActiveUserDTOFromSrv(const UserDTO &user_dto) const;
  void setUserDTOFromSrv(const UserDTO &user_dto) const;
  void setOneMessageDTO(const MessageDTO &message_dto, const std::shared_ptr<Chat> &chat) const;
  bool setOneChatMessageDTO(const MessageChatDTO &message_chat_dto) const;
  void setOneChatDTOFromSrv(const ChatDTO &chat_dto);

private:
  ClientCore &core_;
};
