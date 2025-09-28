#pragma once

#include <memory>
#include <vector>

class ClientSession;
class Chat;
struct UserDTO;
struct MessageDTO;
struct MessageChatDTO;
struct ChatDTO;

class ClientSessionDtoFromSrv {
public:
  explicit ClientSessionDtoFromSrv(ClientSession &session);

  void setActiveUserDTOFromSrv(const UserDTO &userDTO) const;
  void setUserDTOFromSrv(const UserDTO &userDTO) const;
  void setOneMessageDTO(const MessageDTO &messageDTO, const std::shared_ptr<Chat> &chat) const;
  bool setOneChatMessageDTO(const MessageChatDTO &messageChatDTO) const;
  void setOneChatDTOFromSrv(const ChatDTO &chatDTO);
  bool checkAndAddParticipantToSystem(const std::vector<std::string> &participants);

private:
  ClientSession &_session;
};
