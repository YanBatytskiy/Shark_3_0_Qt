#pragma once

#include <cstddef>
#include <memory>
#include <optional>

class ClientSession;
class Chat;
class Message;
struct MessageDTO;
struct ChatDTO;

class ClientSessionDtoBuilder {
public:
  explicit ClientSessionDtoBuilder(ClientSession &session);

  MessageDTO fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chat_id);
  std::optional<ChatDTO> fillChatDTOQt(const std::shared_ptr<Chat> &chat);

private:
  ClientSession &_session;
};
