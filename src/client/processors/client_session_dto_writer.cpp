#include "client/processors/client_session_dto_writer.h"

#include <memory>
#include <vector>

#include "chat/chat.h"
#include "chat_system/chat_system.h"
#include "client/client_session.h"
#include "core/system/system_function.h"
#include "dto_struct.h"
#include "message/message.h"
#include "system/date_time_utils.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"

#include <QString>

ClientSessionDtoWriter::ClientSessionDtoWriter(ClientSession &session) : _session(session) {}

void ClientSessionDtoWriter::setActiveUserDTOFromSrv(const UserDTO &userDTO) const {
  auto user_ptr = std::make_shared<User>(UserData(userDTO.login, userDTO.userName, userDTO.passwordhash, userDTO.email,
                                                  userDTO.phone, userDTO.disable_reason, userDTO.is_active,
                                                  userDTO.disabled_at, userDTO.ban_until));
  user_ptr->createChatList(std::make_shared<UserChatList>(user_ptr));

  auto &instance = _session.getInstance();

  if (!instance.findUserByLogin(userDTO.login)) {
    instance.addUserToSystem(user_ptr);
  }

  instance.setActiveUser(user_ptr);
}

void ClientSessionDtoWriter::setUserDTOFromSrv(const UserDTO &userDTO) const {
  auto user_ptr = std::make_shared<User>(UserData(userDTO.login, userDTO.userName, "-1", userDTO.email, userDTO.phone,
                                                  userDTO.disable_reason, userDTO.is_active, userDTO.disabled_at,
                                                  userDTO.ban_until));
  user_ptr->createChatList();

  auto &instance = _session.getInstance();

  if (!instance.findUserByLogin(userDTO.login)) {
    instance.addUserToSystem(user_ptr);
  }
}

void ClientSessionDtoWriter::setOneMessageDTO(const MessageDTO &messageDTO, const std::shared_ptr<Chat> &chat) const {
  auto sender = _session.getInstance().findUserByLogin(messageDTO.senderLogin);

  auto message = createOneMessage(messageDTO.messageContent[0].payload, sender, messageDTO.timeStamp,
                                  messageDTO.messageId);

  chat->addMessageToChat(std::make_shared<Message>(message), sender, false);
}

bool ClientSessionDtoWriter::setOneChatMessageDTO(const MessageChatDTO &messageChatDTO) const {
  const auto &activeUser = _session.getInstance().getActiveUser();
  if (!activeUser) {
    return false;
  }

  const auto &chatList = activeUser->getUserChatList();
  if (!chatList) {
    return false;
  }

  auto chats = chatList->getChatFromList();

  std::shared_ptr<Chat> chat_ptr;
  for (const auto &chat : chats) {
    chat_ptr = chat.lock();

    if (chat_ptr && chat_ptr->getChatId() == messageChatDTO.chatId) {
      for (const auto &message : messageChatDTO.messageDTO) {
        setOneMessageDTO(message, chat_ptr);
      }
    }
  }
  return true;
}

void ClientSessionDtoWriter::setOneChatDTOFromSrv(const ChatDTO &chatDTO) {
  auto chat_ptr = std::make_shared<Chat>();
  auto activeUser = _session.getActiveUserCl();
  if (!activeUser || !activeUser->getUserChatList()) {
    return;
  }

  chat_ptr->setChatId(chatDTO.chatId);

  std::vector<std::string> participants;
  participants.reserve(chatDTO.participants.size());
  for (const auto &participant : chatDTO.participants) {
    participants.push_back(participant.login);
  }

  _session.checkAndAddParticipantToSystem(participants);

  for (const auto &participant : chatDTO.participants) {
    for (const auto &delMessId : participant.deletedMessageIds) {
      chat_ptr->setDeletedMessageMap(participant.login, delMessId);
    }

    auto user_ptr = _session.getInstance().findUserByLogin(participant.login);

    if (user_ptr) {
      chat_ptr->setLastReadMessageId(user_ptr, participant.lastReadMessage);
      chat_ptr->addParticipant(user_ptr, participant.lastReadMessage, participant.deletedFromChat);
    }
  }

  _session.getInstance().addChatToInstance(chat_ptr);
}
