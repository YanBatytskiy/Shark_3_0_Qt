#pragma once

#include <memory>

class ClientSession;
class Chat;
struct ChatDTO;
struct MessageChatDTO;
struct MessageDTO;
struct UserDTO;

class ClientSessionDtoWriter {
public:
  explicit ClientSessionDtoWriter(ClientSession &session);

  void setActiveUserDTOFromSrv(const UserDTO &userDTO) const;
  void setUserDTOFromSrv(const UserDTO &userDTO) const;
  void setOneMessageDTO(const MessageDTO &messageDTO, const std::shared_ptr<Chat> &chat) const;
  bool setOneChatMessageDTO(const MessageChatDTO &messageChatDTO) const;
  void setOneChatDTOFromSrv(const ChatDTO &chatDTO);

private:
  ClientSession &_session;
};
