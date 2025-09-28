#include "client/processors/client_session_create_objects.h"

#include <memory>
#include <vector>

#include "chat/chat.h"
#include "client/client_session.h"
#include "dto_struct.h"
#include "exceptions_qt/exception_network.h"
#include "exceptions_qt/exception_router.h"
#include "exceptions_qt/exception_valid.h"
#include "message/message.h"
#include "system/date_time_utils.h"
#include "system/system_function.h"
#include "user/user.h"

#include <QString>

ClientSessionCreateObjects::ClientSessionCreateObjects(ClientSession &session) : _session(session) {}

bool ClientSessionCreateObjects::createUserCl(const UserDTO &userDTO) {
  PacketDTO packetDTO;
  packetDTO.requestType = RequestType::RqFrClientCreateUser;
  packetDTO.structDTOClassType = StructDTOClassType::userDTO;
  packetDTO.reqDirection = RequestDirection::ClientToSrv;
  packetDTO.structDTOPtr = std::make_shared<StructDTOClass<UserDTO>>(userDTO);

  std::vector<PacketDTO> packetDTOListSend;
  packetDTOListSend.push_back(packetDTO);

  PacketListDTO packetListDTOresult;
  packetListDTOresult.packets.clear();

  packetListDTOresult = _session.processingRequestToServer(packetDTOListSend, packetDTO.requestType);

  const auto &packet = static_cast<const StructDTOClass<ResponceDTO> &>(*packetListDTOresult.packets[0].structDTOPtr)
                           .getStructDTOClass();

  if (packet.reqResult) {
    return true;
  }
  return false;
}

bool ClientSessionCreateObjects::createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chatDTO,
                                                MessageChatDTO &messageChatDTO) {
  PacketDTO chatPacket;
  chatPacket.requestType = RequestType::RqFrClientCreateChat;
  chatPacket.structDTOClassType = StructDTOClassType::chatDTO;
  chatPacket.reqDirection = RequestDirection::ClientToSrv;
  chatPacket.structDTOPtr = std::make_shared<StructDTOClass<ChatDTO>>(chatDTO);

  std::vector<PacketDTO> packetDTOListSend;
  packetDTOListSend.push_back(chatPacket);

  PacketDTO messagePacket;
  messagePacket.requestType = RequestType::RqFrClientCreateChat;
  messagePacket.structDTOClassType = StructDTOClassType::messageChatDTO;
  messagePacket.reqDirection = RequestDirection::ClientToSrv;
  messagePacket.structDTOPtr = std::make_shared<StructDTOClass<MessageChatDTO>>(messageChatDTO);
  packetDTOListSend.push_back(messagePacket);

  PacketListDTO responcePacketListDTO;
  responcePacketListDTO.packets.clear();

  responcePacketListDTO = _session.processingRequestToServer(packetDTOListSend, RequestType::RqFrClientCreateChat);

  try {
    if (responcePacketListDTO.packets.empty()) {
      throw exc_qt::EmptyPacketException();
    }

    if (responcePacketListDTO.packets[0].requestType != RequestType::RqFrClientCreateChat) {
      throw exc_qt::WrongresponceTypeException();
    }

    const auto &packetDTO = static_cast<const StructDTOClass<ResponceDTO> &>(*responcePacketListDTO.packets[0].structDTOPtr)
                                .getStructDTOClass();

    if (!packetDTO.reqResult) {
      throw exc_qt::CreateChatException();
    }

    auto generalChatId = packetDTO.anyNumber;
    chat->setChatId(generalChatId);

    auto generalMessageId = parseGetlineToSizeT(packetDTO.anyString);

    if (chat->getMessages().empty()) {
      throw exc_qt::CreateMessageException();
    }

    chat->getMessages().begin()->second->setMessageId(generalMessageId);

  } catch (const exc_qt::WrongresponceTypeException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::EmptyPacketException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::CreateChatException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::CreateChatIdException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::CreateMessageIdException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  } catch (const exc_qt::CreateMessageException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(chatDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(chatDTO.chatId));
    const auto messageIdQt = QString::number(0);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createNewChatCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return false;
  }

  return true;
}

std::size_t ClientSessionCreateObjects::createMessageCl(const Message &message, std::shared_ptr<Chat> &chat_ptr,
                                                       const std::shared_ptr<User> &user) {
  auto message_ptr = std::make_shared<Message>(message);

  MessageDTO messageDTO = _session.fillOneMessageDTOFromCl(message_ptr, chat_ptr->getChatId());

  PacketDTO packetDTO;
  packetDTO.requestType = RequestType::RqFrClientCreateMessage;
  packetDTO.structDTOClassType = StructDTOClassType::messageDTO;
  packetDTO.reqDirection = RequestDirection::ClientToSrv;
  packetDTO.structDTOPtr = std::make_shared<StructDTOClass<MessageDTO>>(messageDTO);

  std::vector<PacketDTO> packetDTOListForSendVector;
  packetDTOListForSendVector.push_back(packetDTO);

  PacketListDTO responcePacketListDTO;
  responcePacketListDTO.packets.clear();

  responcePacketListDTO = _session.processingRequestToServer(packetDTOListForSendVector, packetDTO.requestType);

  std::size_t newMessageId = 0;

  try {
    if (responcePacketListDTO.packets.empty()) {
      throw exc_qt::EmptyPacketException();
    }

    if (responcePacketListDTO.packets.size() > 1) {
      throw exc_qt::WrongPacketSizeException();
    }

    if (responcePacketListDTO.packets[0].requestType != RequestType::RqFrClientCreateMessage) {
      throw exc_qt::WrongresponceTypeException();
    }

    const auto &packetDTO = static_cast<const StructDTOClass<ResponceDTO> &>(
                                *responcePacketListDTO.packets[0].structDTOPtr)
                                .getStructDTOClass();

    if (!packetDTO.reqResult) {
      throw exc_qt::CreateMessageException();
    }

    newMessageId = parseGetlineToSizeT(packetDTO.anyString);

    if (chat_ptr->getMessages().empty()) {
      throw exc_qt::CreateMessageException();
    }

    message_ptr->setMessageId(newMessageId);

    chat_ptr->addMessageToChat(message_ptr, user, false);

  } catch (const exc_qt::EmptyPacketException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createMessageCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return 0;
  } catch (const exc_qt::WrongPacketSizeException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createMessageCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return 0;
  } catch (const exc_qt::WrongresponceTypeException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createMessageCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return 0;
  } catch (const exc_qt::CreateMessageException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createMessageCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return 0;
  } catch (const exc_qt::ValidationException &ex) {
    const auto time_stamp = QString::fromStdString(formatTimeStampToString(getCurrentDateTimeInt(), true));
    const auto senderLoginQt = QString::fromStdString(messageDTO.senderLogin);
    const auto chatIdQt = QString::number(static_cast<long long>(messageDTO.chatId));
    const auto messageIdQt = QString::number(static_cast<long long>(messageDTO.messageId));

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral(
                                         "[%1]   [ERROR]   [NETWORK]   [user=%2]   [chat_Id=%3]   [msg=%4]   createMessageCl   ")
                                         .arg(time_stamp, senderLoginQt, chatIdQt, messageIdQt));
    return 0;
  }
  return newMessageId;
}
