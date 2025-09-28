#include "client_session.h"

#include "chat/chat.h"
#include "chat_system/chat_system.h"
#include "client/core/client_core.h"
#include "dto/dto_struct.h"
#include "exceptions_cpp/login_exception.h"
#include "exceptions_qt/exception_login.h"
#include "exceptions_qt/exception_network.h"
#include "exceptions_qt/exception_router.h"
#include "message/message_content_struct.h"
#include "system/date_time_utils.h"
#include "system/serialize.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"

#include <QString>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include <poll.h>
#include <unistd.h>

#ifndef POLLRDHUP
#define POLLRDHUP 0
#endif

// constructor
ClientSession::ClientSession(ChatSystem &chat_system, QObject *parent)
    : QObject(parent), chat_system_(chat_system),
      client_core_(std::make_shared<ClientCore>(chat_system_, this)) {
  QObject::connect(client_core_.get(), &ClientCore::serverStatusChanged, this, &ClientSession::serverStatusChanged);
}

ClientSession::~ClientSession() { stopConnectionThread(); }

bool ClientSession::getIsServerOnline() const noexcept { return client_core_->getIsServerOnlineCore(); }

bool ClientSession::inputNewLoginValidationQt(std::string inputData) {

  // проверяем только на англ буквы и цифры
  if (!engAndFiguresCheck(inputData))
    return false;
  else
    return true;
}

bool ClientSession::inputNewPasswordValidationQt(std::string inputData, std::size_t dataLengthMin,
                                                 std::size_t dataLengthMax) {

  // проверяем только на англ буквы и цифры
  if (!engAndFiguresCheck(inputData))
    return false;

  if (!checkNewLoginPasswordForLimits(inputData, dataLengthMin, dataLengthMax, true))
    return false;
  else
    return true;
}

std::optional<std::multimap<std::int64_t, ChatDTO, std::greater<std::int64_t>>> ClientSession::getChatListCl() {
  return client_core_->getChatListCl();
}

bool ClientSession::CreateAndSendNewChatCl(std::shared_ptr<Chat> &chat_ptr, std::vector<UserDTO> &participants,
                                           Message &new_message) {
  return client_core_->CreateAndSendNewChatCl(chat_ptr, participants, new_message);
}

bool ClientSession::changeUserPasswordCl(UserDTO user_dto) { return client_core_->changeUserPasswordCl(user_dto); }

bool ClientSession::blockUnblockUserCl(const std::string &login, bool is_blocked, const std::string &disable_reason) {
  return client_core_->blockUnblockUserCl(login, is_blocked, disable_reason);
}

bool ClientSession::bunUnbunUserCl(const std::string &login, bool is_banned, std::int64_t banned_to) {
  return client_core_->bunUnbunUserCl(login, is_banned, banned_to);
}

// threads
void ClientSession::startConnectionThread() {
  if (connection_thread_running_.exchange(true, std::memory_order_acq_rel)) {
    return;
  }
  connection_thread_ = std::thread([this] { connectionMonitorLoop(); });
}

void ClientSession::stopConnectionThread() {
  if (!connection_thread_running_.exchange(false, std::memory_order_acq_rel)) {
    return;
  }
  if (connection_thread_.joinable()) {
    connection_thread_.join();
  }
}

// getters

ServerConnectionConfig &ClientSession::getserverConnectionConfigCl() {
  return client_core_->getServerConnectionConfigCore();
}

const ServerConnectionConfig &ClientSession::getserverConnectionConfigCl() const {
  return client_core_->getServerConnectionConfigCore();
}

ServerConnectionMode &ClientSession::getserverConnectionModeCl() { return client_core_->getServerConnectionModeCore(); }

const ServerConnectionMode &ClientSession::getserverConnectionModeCl() const {
  return client_core_->getServerConnectionModeCore();
}

const std::shared_ptr<User> ClientSession::getActiveUserCl() const { return client_core_->getActiveUserCl(); }

ChatSystem &ClientSession::getInstance() { return client_core_->getInstance(); }

std::size_t ClientSession::getSocketFd() const {
  return static_cast<std::size_t>(client_core_->getSocketFdCore());
}

void ClientSession::setActiveUserCl(const std::shared_ptr<User> &user) { client_core_->setActiveUserCl(user); }

void ClientSession::setSocketFd(int socket_fd) { client_core_->setSocketFdCore(socket_fd); }

//
//
//
// checking and finding
//
//
//
const std::vector<UserDTO> ClientSession::findUserByTextPartOnServerCl(const std::string &text_to_find) {
  return client_core_->findUserByTextPartOnServerCl(text_to_find);
}
//
//
//
bool ClientSession::checkUserLoginCl(const std::string &user_login) {
  return client_core_->checkUserLoginCl(user_login);
}
//
//
//
bool ClientSession::checkUserPasswordCl(const std::string &user_login, const std::string &password) {
  return client_core_->checkUserPasswordCl(user_login, password);
}
// transport
//
//
bool ClientSession::findServerAddress(ServerConnectionConfig &server_connection_config,
                                      ServerConnectionMode &server_connection_mode) {
  return client_core_->findServerAddressCore(server_connection_config, server_connection_mode);
}
//
//
//

int ClientSession::createConnection(ServerConnectionConfig &server_connection_config,
                                    ServerConnectionMode &server_connection_mode) {
  return client_core_->createConnectionCore(server_connection_config, server_connection_mode);
}
//
//
//
bool ClientSession::discoverServerOnLAN(ServerConnectionConfig &server_connection_config) {
  return client_core_->discoverServerOnLANCore(server_connection_config);
}
//
//
//
PacketListDTO ClientSession::getDatafromServer(const std::vector<std::uint8_t> &packet_list_send) {
  return client_core_->getDatafromServerCore(packet_list_send);
}

//
//
PacketListDTO ClientSession::processingRequestToServer(std::vector<PacketDTO> &packet_dto_vector,
                                                       const RequestType &request_type) {
  return client_core_->processingRequestToServerCore(packet_dto_vector, request_type);
}

//
//
//
// utilities

bool ClientSession::initServerConnection() { return client_core_->initServerConnectionCore(); }

void ClientSession::resetSessionData() { client_core_->resetSessionDataCore(); }

bool ClientSession::reInitilizeBaseCl() { return client_core_->reInitilizeBaseCl(); }

//
//
//
bool ClientSession::registerClientToSystemCl(const std::string &login) { return client_core_->registerClientToSystemCl(login); }


//
//
//

bool ClientSession::changeUserDataCl(const UserDTO &user_dto) { return client_core_->changeUserDataCl(user_dto); }

bool ClientSession::createUserCl(const UserDTO &user_dto) { return client_core_->createUserCl(user_dto); }

bool ClientSession::createNewChatCl(std::shared_ptr<Chat> &chat, ChatDTO &chat_dto, MessageChatDTO &message_chat_dto) {
  return client_core_->createNewChatCl(chat, chat_dto, message_chat_dto);
}
//
//
//
MessageDTO ClientSession::fillOneMessageDTOFromCl(const std::shared_ptr<Message> &message, std::size_t chat_id) { return client_core_->fillOneMessageDTOFromCl(message, chat_id); }

//
//
//
std::size_t ClientSession::createMessageCl(const Message &message, std::shared_ptr<Chat> &chat_ptr, const std::shared_ptr<User> &user) { return client_core_->createMessageCl(message, chat_ptr, user); }

//
//
//
// отправка пакета LastReadMessage
bool ClientSession::sendLastReadMessageFromClient(const std::shared_ptr<Chat> &chat_ptr, std::size_t message_id) { return client_core_->sendLastReadMessageFromClient(chat_ptr, message_id); }

bool ClientSession::checkAndAddParticipantToSystem(const std::vector<std::string> &participants) { return client_core_->ensureParticipantsAvailable(participants); }

std::optional<ChatDTO> ClientSession::fillChatDTOCl(const std::shared_ptr<Chat> &chat_ptr) {
  return client_core_->fillChatDTOCl(chat_ptr);
}

std::shared_ptr<ClientCore> ClientSession::getClientCore() const { return client_core_; }

void ClientSession::connectionMonitorLoop() {
  try {
    bool online = client_core_->getIsServerOnlineCore();

    while (connection_thread_running_.load(std::memory_order_acquire)) {
      if (!online) {
        auto &config = client_core_->getServerConnectionConfigCore();
        auto &mode_ref = client_core_->getServerConnectionModeCore();

        if (!client_core_->findServerAddressCore(config, mode_ref)) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          continue;
        }

        const int fd = client_core_->createConnectionCore(config, mode_ref);

        if (fd < 0) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          continue;
        }

        online = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        continue;
      }

      const int fd = client_core_->getSocketFdCore();
      if (!socketAlive(fd)) {
        if (fd >= 0) {
          ::close(fd);
        }
        client_core_->setSocketFdCore(-1);
        online = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        continue;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  } catch (const std::exception &) {
    connection_thread_running_.store(false, std::memory_order_release);
    return;
  }
}

bool ClientSession::socketAlive(int fd) {
  if (fd < 0) {
    return false;
  }

  pollfd descriptor{};
  descriptor.fd = fd;
  descriptor.events = POLLIN | POLLERR | POLLHUP | POLLRDHUP;

  const int result = ::poll(&descriptor, 1, 0);

  if (result < 0) {
    return false;
  }

  if (result == 0) {
    return true;
  }

  if (descriptor.revents & (POLLERR | POLLHUP | POLLRDHUP)) {
    return false;
  }

  return true;
}
