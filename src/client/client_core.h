#pragma once

#include "chat_system/chat_system.h"
#include "dto_struct.h"
#include "request_dispatcher/request_dispatcher.h"
#include "tcp_transport/session_types.h"
#include "tcp_transport/tcp_transport.h"

#include <QThread>
#include <QObject>

#include <atomic>
#include <vector>

class ConnectionMonitor;

class ClientCore : public QObject {
  Q_OBJECT
public:
  ClientCore(ChatSystem &chat_system, QObject *parent = nullptr);
  ClientCore(const ClientCore &) = delete;
  ClientCore &operator=(const ClientCore &) = delete;
  ClientCore(ClientCore &&) = delete;
  ClientCore &operator=(ClientCore &&) = delete;

  ~ClientCore();

  // lifecycle
  void startConnectionThreadCore();
  void stopConnectionThreadCore();

  // state access
  ConnectionMonitor *getConnectionMonitorCore();
  ServerConnectionConfig &getServerConnectionConfigCore();
  const ServerConnectionConfig &getServerConnectionConfigCore() const;
  ServerConnectionMode &getServerConnectionModeCore();
  const ServerConnectionMode &getServerConnectionModeCore() const;
  bool getIsServerOnlineCore() const noexcept;
  int getSocketFdCore() const;
  void setSocketFdCore(int socket_fd);

  // transport helpers
  bool findServerAddressCore(ServerConnectionConfig &serverConnectionConfig, ServerConnectionMode &serverConnectionMode);
  int createConnectionCore(ServerConnectionConfig &serverConnectionConfig, ServerConnectionMode &serverConnectionMode);
  bool discoverServerOnLANCore(ServerConnectionConfig &serverConnectionConfig);
  PacketListDTO getDatafromServerCore(const std::vector<std::uint8_t> &packetListSend);
  PacketListDTO processingRequestToServerCore(std::vector<PacketDTO> &packetDTOListVector, const RequestType &requestType);

  // utilities
  bool initServerConnectionCore();
  void resetSessionDataCore();

signals:
  void serverStatusChanged(bool online, ServerConnectionMode mode);

public slots:
  void onConnectionStateChangedCore(bool online, ServerConnectionMode mode);

private:
  ChatSystem &chat_system_;
  ServerConnectionConfig server_connection_config_{};
  ServerConnectionMode server_connection_mode_{ServerConnectionMode::Offline};
  int socket_fd_{-1};

  QThread net_thread_;
  ConnectionMonitor *connection_monitor_{nullptr};
  std::atomic_bool status_online_{false};
  TcpTransport transport_;
  RequestDispatcher dispatcher_;
};
