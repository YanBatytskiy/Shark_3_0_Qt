#pragma once

#include <cstddef>
#include <memory>
#include <optional>

class ClientCore;
class Chat;
class Message;
struct MessageDTO;
struct ChatDTO;

class ClientSessionDtoBuilder {
public:
  explicit ClientSessionDtoBuilder(ClientCore &core);

  MessageDTO fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chat_id);
  std::optional<ChatDTO> fillChatDTOCl(const std::shared_ptr<Chat> &chat);

private:
  ClientCore &core_;
};
