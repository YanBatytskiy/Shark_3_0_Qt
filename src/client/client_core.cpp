#include "client_core.h"

#include "nw_connection_monitor.h"

#include <unistd.h>

ClientCore::ClientCore(ChatSystem &chat_system, QObject *parent)
    : QObject(parent), chat_system_(chat_system), dispatcher_(chat_system_, transport_) {}

ClientCore::~ClientCore() {
  stopConnectionThreadCore();
  if (socket_fd_ >= 0) {
    ::close(socket_fd_);
    socket_fd_ = -1;
  }
}

void ClientCore::startConnectionThreadCore() {
  if (connection_monitor_)
    return;

  connection_monitor_ = new ConnectionMonitor(this);
  connection_monitor_->moveToThread(&net_thread_);

  QObject::connect(&net_thread_, &QThread::started, connection_monitor_, &ConnectionMonitor::run);
  QObject::connect(&net_thread_, &QThread::finished, connection_monitor_, &QObject::deleteLater);
  QObject::connect(connection_monitor_, &ConnectionMonitor::connectionStateChanged, this,
                   &ClientCore::onConnectionStateChangedCore, Qt::QueuedConnection);

  net_thread_.start();
}

void ClientCore::stopConnectionThreadCore() {
  if (!connection_monitor_)
    return;

  connection_monitor_->stop();
  net_thread_.quit();
  net_thread_.wait();
  connection_monitor_ = nullptr;
}

void ClientCore::onConnectionStateChangedCore(bool online, ServerConnectionMode mode) {
  status_online_.store(online, std::memory_order_release);
  server_connection_mode_ = mode;
  emit serverStatusChanged(online, mode);
}

ConnectionMonitor *ClientCore::getConnectionMonitorCore() { return connection_monitor_; }

ServerConnectionConfig &ClientCore::getServerConnectionConfigCore() { return server_connection_config_; }

const ServerConnectionConfig &ClientCore::getServerConnectionConfigCore() const { return server_connection_config_; }

ServerConnectionMode &ClientCore::getServerConnectionModeCore() { return server_connection_mode_; }

const ServerConnectionMode &ClientCore::getServerConnectionModeCore() const { return server_connection_mode_; }

bool ClientCore::getIsServerOnlineCore() const noexcept { return status_online_.load(std::memory_order_acquire); }

int ClientCore::getSocketFdCore() const { return socket_fd_; }

void ClientCore::setSocketFdCore(int socket_fd) { socket_fd_ = socket_fd; }

bool ClientCore::findServerAddressCore(ServerConnectionConfig &config, ServerConnectionMode &mode) {
  return transport_.findServerAddress(config, mode);
}

int ClientCore::createConnectionCore(ServerConnectionConfig &config, ServerConnectionMode &mode) {
  return transport_.createConnection(config, mode);
}

bool ClientCore::discoverServerOnLANCore(ServerConnectionConfig &config) {
  return transport_.discoverServerOnLAN(config);
}

PacketListDTO ClientCore::getDatafromServerCore(const std::vector<std::uint8_t> &payload) {
  return transport_.transportPackets(socket_fd_, status_online_, payload);
}

PacketListDTO ClientCore::processingRequestToServerCore(std::vector<PacketDTO> &packets,
                                                        const RequestType &request_type) {
  return dispatcher_.process(socket_fd_, status_online_, packets, request_type);
}

bool ClientCore::initServerConnectionCore() {
  chat_system_.setIsServerStatus(false);

  if (!findServerAddressCore(server_connection_config_, server_connection_mode_)) {
    server_connection_mode_ = ServerConnectionMode::Offline;
    emit serverStatusChanged(false, server_connection_mode_);
    return false;
  }

  const int fd = createConnectionCore(server_connection_config_, server_connection_mode_);

  if (fd < 0) {
    server_connection_mode_ = ServerConnectionMode::Offline;
    emit serverStatusChanged(false, server_connection_mode_);
    return false;
  }

  socket_fd_ = fd;
  status_online_.store(true, std::memory_order_release);
  chat_system_.setIsServerStatus(true);
  emit serverStatusChanged(true, server_connection_mode_);
  return true;
}

void ClientCore::resetSessionDataCore() {
  chat_system_ = ChatSystem();
  socket_fd_ = -1;
  status_online_.store(false, std::memory_order_release);
  server_connection_mode_ = ServerConnectionMode::Offline;
  server_connection_config_.found = false;
}
