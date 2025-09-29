#include "client/core/client_core.h"

#include <poll.h>
#include <unistd.h>

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

// === monitoring ===
void ClientCore::connectionMonitorLoopCore() {
  try {
    bool online = getIsServerOnlineCore();

    while (connection_thread_running_.load(std::memory_order_acquire)) {
      if (!online) {
        auto &config = getServerConnectionConfigCore();
        auto &mode_ref = getServerConnectionModeCore();

        if (!findServerAddressCore(config, mode_ref)) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          continue;
        }

        const int fd = createConnectionCore(config, mode_ref);

        if (fd < 0) {
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
          continue;
        }

        online = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        continue;
      }

      const int fd = getSocketFdCore();
      if (!socketAliveCore(fd)) {
        if (fd >= 0) {
          ::close(fd);
        }
        setSocketFdCore(-1);
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

bool ClientCore::socketAliveCore(int fd) {
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
