#include "client_session_dto_from_srv.h"

#include <iostream>
#include <memory>
#include <vector>

#include "chat/chat.h"
#include "chat_system/chat_system.h"
#include "client_session.h"
#include "core/system/system_function.h"
#include "dto_struct.h"
#include "exceptions_qt/exception_network.h"
#include "exceptions_qt/exception_router.h"
#include "message/message.h"
#include "system/date_time_utils.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"

#include <QString>

ClientSessionDtoFromSrv::ClientSessionDtoFromSrv(ClientSession &session) : _session(session) {}

void ClientSessionDtoFromSrv::setActiveUserDTOFromSrv(const UserDTO &userDTO) const {
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

void ClientSessionDtoFromSrv::setUserDTOFromSrv(const UserDTO &userDTO) const {
  auto user_ptr = std::make_shared<User>(UserData(userDTO.login, userDTO.userName, "-1", userDTO.email, userDTO.phone,
                                                  userDTO.disable_reason, userDTO.is_active, userDTO.disabled_at,
                                                  userDTO.ban_until));
  user_ptr->createChatList();

  auto &instance = _session.getInstance();

  if (!instance.findUserByLogin(userDTO.login)) {
    instance.addUserToSystem(user_ptr);
  }
}

void ClientSessionDtoFromSrv::setOneMessageDTO(const MessageDTO &messageDTO, const std::shared_ptr<Chat> &chat) const {
  auto sender = _session.getInstance().findUserByLogin(messageDTO.senderLogin);

  auto message = createOneMessage(messageDTO.messageContent[0].payload, sender, messageDTO.timeStamp,
                                  messageDTO.messageId);

  chat->addMessageToChat(std::make_shared<Message>(message), sender, false);
}

bool ClientSessionDtoFromSrv::setOneChatMessageDTO(const MessageChatDTO &messageChatDTO) const {
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

void ClientSessionDtoFromSrv::setOneChatDTOFromSrv(const ChatDTO &chatDTO) {
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

  checkAndAddParticipantToSystem(participants);

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

bool ClientSessionDtoFromSrv::checkAndAddParticipantToSystem(const std::vector<std::string> &participants) {
  try {
    bool needRequest = false;
    std::vector<PacketDTO> packetDTOListSend;

    for (const auto &participant : participants) {
      const auto &user_ptr = _session.getInstance().findUserByLogin(participant);

      if (user_ptr == nullptr) {
        needRequest = true;

        UserLoginDTO userLoginDTO;
        userLoginDTO.login = participant;

        PacketDTO packetDTO;
        packetDTO.requestType = RequestType::RqFrClientGetUsersData;
        packetDTO.structDTOClassType = StructDTOClassType::userLoginDTO;
        packetDTO.reqDirection = RequestDirection::ClientToSrv;
        packetDTO.structDTOPtr = std::make_shared<StructDTOClass<UserLoginDTO>>(userLoginDTO);

        packetDTOListSend.push_back(packetDTO);
      }
    }

    if (needRequest) {
      PacketListDTO packetListDTOresult;
      packetListDTOresult.packets.clear();

      packetListDTOresult = _session.processingRequestToServer(packetDTOListSend, RequestType::RqFrClientGetUsersData);

      {
        for (const auto &responcePacket : packetListDTOresult.packets) {
          if (responcePacket.requestType != RequestType::RqFrClientGetUsersData) {
            continue;
          }

          if (responcePacket.structDTOClassType != StructDTOClassType::userDTO) {
            throw exc_qt::WrongresponceTypeException();
          }

          const auto &userDTO = static_cast<const StructDTOClass<UserDTO> &>(*responcePacket.structDTOPtr)
                                    .getStructDTOClass();

          auto newUser_ptr = std::make_shared<User>(UserData(userDTO.login, userDTO.userName, "-1", userDTO.email,
                                                             userDTO.phone, userDTO.disable_reason, userDTO.is_active,
                                                             userDTO.disabled_at, userDTO.ban_until));

          _session.getInstance().addUserToSystem(newUser_ptr);
        }
      }
    } else {
      const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
      const auto timeStampQt = QString::fromStdString(time_sdtamp);

      emit exc_qt::ErrorBus::i().error("",
                                       QStringLiteral(
                                           "[%1]   checkAndAddParticipantToSystem   server answer did not come")
                                           .arg(timeStampQt));

      for (const auto &packetDTO : packetDTOListSend) {
        const auto &userLoginDTO = static_cast<const StructDTOClass<UserLoginDTO> &>(*packetDTO.structDTOPtr)
                                       .getStructDTOClass();

        auto newUser_ptr = std::make_shared<User>(
            UserData(userLoginDTO.login, "Unknown", "-1", "Unknown", "Unknown", "", true, 0, 0));

        _session.getInstance().addUserToSystem(newUser_ptr);
      }
    }
  } catch (const exc_qt::WrongresponceTypeException &ex) {
    const auto time_sdtamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto timeStampQt = QString::fromStdString(time_sdtamp);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=]   checkAndAddParticipantToSystem   Wrong responce type")
                                         .arg(timeStampQt));

    std::cerr << "Клиент: добавление участников. " << ex.what() << std::endl;
    return false;
  }
  return true;
}
