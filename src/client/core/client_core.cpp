#include "client/core/client_core.h"

#include <netinet/in.h>

#include <QString>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "chat/chat.h"
#include "chat_system/chat_system.h"
#include "dto/dto_struct.h"
#include "exceptions_cpp/login_exception.h"
#include "exceptions_qt/exception_login.h"
#include "exceptions_qt/exception_network.h"
#include "exceptions_qt/exception_router.h"
#include "system/date_time_utils.h"
#include "system/picosha2.h"
#include "system/serialize.h"
#include "system/system_function.h"
#include "user/user.h"
#include "user/user_chat_list.h"

namespace {
constexpr int kInvalidSocket = -1;
}

ClientCore::ClientCore(ChatSystem &chat_system, QObject *parent)
    : QObject(parent),
      chat_system_(chat_system),
      transport_(),
      request_dispatcher_(chat_system_, transport_) {}

ClientCore::~ClientCore() = default;

bool ClientCore::getIsServerOnlineCore() const noexcept {
  return status_online_.load(std::memory_order_acquire);
}

ServerConnectionConfig &ClientCore::getServerConnectionConfigCore() {
  return server_connection_config_;
}

const ServerConnectionConfig &ClientCore::getServerConnectionConfigCore()
    const {
  return server_connection_config_;
}

ServerConnectionMode &ClientCore::getServerConnectionModeCore() {
  return server_connection_mode_;
}

const ServerConnectionMode &ClientCore::getServerConnectionModeCore() const {
  return server_connection_mode_;
}

int ClientCore::getSocketFdCore() const {
  std::scoped_lock lock(socket_mutex_);
  return socket_fd_;
}

void ClientCore::setSocketFdCore(int socket_fd) {
  {
    std::scoped_lock lock(socket_mutex_);
    socket_fd_ = socket_fd;
  }

  if (socket_fd_ < 0) {
    updateConnectionStateCore(false, ServerConnectionMode::Offline);
  } else {
    updateConnectionStateCore(true, server_connection_mode_);
  }
}

const std::vector<UserDTO> ClientCore::findUserByTextPartOnServerCore(
    const std::string &text_to_find) {
  UserLoginPasswordDTO user_login_password_dto;
  user_login_password_dto.login = chat_system_.getActiveUser()->getLogin();
  user_login_password_dto.passwordhash = text_to_find;

  PacketDTO packet_dto;
  packet_dto.requestType = RequestType::RqFrClientFindUserByPart;
  packet_dto.structDTOClassType = StructDTOClassType::userLoginPasswordDTO;
  packet_dto.reqDirection = RequestDirection::ClientToSrv;
  packet_dto.structDTOPtr =
      std::make_shared<StructDTOClass<UserLoginPasswordDTO>>(
          user_login_password_dto);

  std::vector<PacketDTO> packet_list_send;
  packet_list_send.push_back(packet_dto);

  PacketListDTO response_packet_list;
  response_packet_list.packets.clear();

  response_packet_list =
      processingRequestToServerCore(packet_list_send, packet_dto.requestType);

  std::vector<UserDTO> result;
  result.clear();

  for (const auto &packet : response_packet_list.packets) {
    if (packet.requestType != RequestType::RqFrClientFindUserByPart) {
      continue;
    }

    if (!packet.structDTOPtr) {
      continue;
    }

    switch (packet.structDTOClassType) {
      case StructDTOClassType::userDTO: {
        const auto &packet_user_dto =
            static_cast<const StructDTOClass<UserDTO> &>(*packet.structDTOPtr)
                .getStructDTOClass();
        result.push_back(packet_user_dto);
        break;
      }
      case StructDTOClassType::responceDTO: {
        const auto &response = static_cast<const StructDTOClass<ResponceDTO> &>(
                                   *packet.structDTOPtr)
                                   .getStructDTOClass();
        if (!response.reqResult) {
          result.clear();
          return result;
        }
        break;
      }
      default:
        break;
    }
  }
  return result;
}

bool ClientCore::findServerAddressCore(ServerConnectionConfig &config,
                                       ServerConnectionMode &mode) {
  const bool found = transport_.findServerAddress(config, mode);
  if (found) {
    server_connection_config_ = config;
    server_connection_mode_ = mode;
  }
  return found;
}

int ClientCore::createConnectionCore(const ServerConnectionConfig &config,
                                     ServerConnectionMode mode) {
  const int fd = transport_.createConnection(config, mode);
  if (fd >= 0) {
    server_connection_mode_ = mode;
    setSocketFdCore(fd);
  } else {
    updateConnectionStateCore(false, ServerConnectionMode::Offline);
  }
  return fd;
}

bool ClientCore::discoverServerOnLANCore(ServerConnectionConfig &config) {
  return transport_.discoverServerOnLAN(config);
}

PacketListDTO ClientCore::getDatafromServerCore(
    const std::vector<std::uint8_t> &payload) {
  const int socket_fd = getSocketFdCore();
  if (socket_fd < 0) {
    PacketListDTO empty;
    empty.packets.clear();
    return empty;
  }
  return transport_.getDataFromServer(socket_fd, status_online_, payload);
}

PacketListDTO ClientCore::processingRequestToServerCore(
    std::vector<PacketDTO> &packets, const RequestType &request_type) {
  const int socket_fd = getSocketFdCore();
  if (socket_fd < 0) {
    PacketListDTO empty;
    empty.packets.clear();
    return empty;
  }
  return request_dispatcher_.process(socket_fd, status_online_, packets,
                                     request_type);
}

bool ClientCore::initServerConnectionCore() {
  auto config = server_connection_config_;
  auto mode = server_connection_mode_;

  if (!findServerAddressCore(config, mode)) {
    return false;
  }

  const int fd = createConnectionCore(config, mode);
  return fd >= 0;
}

void ClientCore::resetSessionDataCore() {
  chat_system_.setActiveUser(std::shared_ptr<User>{});
  chat_system_.setIsServerStatus(false);
  server_connection_config_ = {};
  server_connection_mode_ = ServerConnectionMode::Offline;
  setSocketFdCore(kInvalidSocket);
}

void ClientCore::updateConnectionStateCore(bool online,
                                           ServerConnectionMode mode) {
  const bool previous =
      status_online_.exchange(online, std::memory_order_acq_rel);
  chat_system_.setIsServerStatus(online);

  if (previous != online || server_connection_mode_ != mode) {
    server_connection_mode_ = mode;
    emit serverStatusChanged(online, mode);
  }
}
