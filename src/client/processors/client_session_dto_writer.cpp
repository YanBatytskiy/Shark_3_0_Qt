#include "client/processors/client_session_dto_writer.h"

#include <memory>
#include <vector>

#include "chat/chat.h"
#include "chat_system/chat_system.h"
#include "client/core/client_core.h"
#include "core/system/system_function.h"
#include "dto_struct.h"
#include "message/message.h"
#include "system/date_time_utils.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"

#include <QString>

ClientSessionDtoWriter::ClientSessionDtoWriter(ClientCore &core) : core_(core) {}

void ClientSessionDtoWriter::setActiveUserDTOFromSrv(const UserDTO &user_dto) const {
  auto user_ptr = std::make_shared<User>(UserData(user_dto.login, user_dto.userName, user_dto.passwordhash, user_dto.email,
                                                  user_dto.phone, user_dto.disable_reason, user_dto.is_active,
                                                  user_dto.disabled_at, user_dto.ban_until));
  user_ptr->createChatList(std::make_shared<UserChatList>(user_ptr));

  auto &instance = core_.getInstance();

  if (!instance.findUserByLogin(user_dto.login)) {
    instance.addUserToSystem(user_ptr);
  }

  instance.setActiveUser(user_ptr);
}

void ClientSessionDtoWriter::setUserDTOFromSrv(const UserDTO &user_dto) const {
  auto user_ptr = std::make_shared<User>(UserData(user_dto.login, user_dto.userName, "-1", user_dto.email, user_dto.phone,
                                                  user_dto.disable_reason, user_dto.is_active, user_dto.disabled_at,
                                                  user_dto.ban_until));
  user_ptr->createChatList();

  auto &instance = core_.getInstance();

  if (!instance.findUserByLogin(user_dto.login)) {
    instance.addUserToSystem(user_ptr);
  }
}

void ClientSessionDtoWriter::setOneMessageDTO(const MessageDTO &message_dto, const std::shared_ptr<Chat> &chat) const {
  auto sender = core_.getInstance().findUserByLogin(message_dto.senderLogin);

  auto message = createOneMessage(message_dto.messageContent[0].payload, sender, message_dto.timeStamp,
                                  message_dto.messageId);

  chat->addMessageToChat(std::make_shared<Message>(message), sender, false);
}

bool ClientSessionDtoWriter::setOneChatMessageDTO(const MessageChatDTO &message_chat_dto) const {
  const auto &active_user = core_.getInstance().getActiveUser();
  if (!active_user) {
    return false;
  }

  const auto &chat_list = active_user->getUserChatList();
  if (!chat_list) {
    return false;
  }

  auto chats = chat_list->getChatFromList();

  std::shared_ptr<Chat> chat_ptr;
  for (const auto &chat : chats) {
    chat_ptr = chat.lock();

    if (chat_ptr && chat_ptr->getChatId() == message_chat_dto.chatId) {
      for (const auto &message : message_chat_dto.messageDTO) {
        setOneMessageDTO(message, chat_ptr);
      }
    }
  }
  return true;
}

void ClientSessionDtoWriter::setOneChatDTOFromSrv(const ChatDTO &chat_dto) {
  auto chat_ptr = std::make_shared<Chat>();
  auto active_user = core_.getActiveUserCl();
  if (!active_user || !active_user->getUserChatList()) {
    return;
  }

  chat_ptr->setChatId(chat_dto.chatId);

  std::vector<std::string> participants;
  participants.reserve(chat_dto.participants.size());
  for (const auto &participant : chat_dto.participants) {
    participants.push_back(participant.login);
  }

  core_.ensureParticipantsAvailable(participants);

  for (const auto &participant : chat_dto.participants) {
    for (const auto &deleted_message_id : participant.deletedMessageIds) {
      chat_ptr->setDeletedMessageMap(participant.login, deleted_message_id);
    }

    auto user_ptr = core_.getInstance().findUserByLogin(participant.login);

    if (user_ptr) {
      chat_ptr->setLastReadMessageId(user_ptr, participant.lastReadMessage);
      chat_ptr->addParticipant(user_ptr, participant.lastReadMessage, participant.deletedFromChat);
    }
  }

  core_.getInstance().addChatToInstance(chat_ptr);
}
