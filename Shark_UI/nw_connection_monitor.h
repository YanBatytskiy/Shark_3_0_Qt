#ifndef NW_CONNECTION_MONITOR_H
#define NW_CONNECTION_MONITOR_H
#include <atomic>
#include "client_session.h"
#include <poll.h>
#include <QObject>

class ConnectionMonitor : public QObject {
  Q_OBJECT
public:
  explicit ConnectionMonitor(ClientSession* session) : _session(session){}

public slots:
  void run();
  void stop(){_run = false;}

signals:
  void connectionStateChanged(bool connectionStatus, ServerConnectionMode serverConnectionMode);

private:
  ClientSession* _session;
  std::atomic_bool _run{true};

  static bool socketAlive(int fd){
    if (fd < 0) return false;
    pollfd p{};
    p.fd = fd;
    p.events = POLLIN | POLLERR | POLLHUP | POLLRDHUP;

    const int result = poll(&p, 1, 0);

    if (result < 0) return false;
    if (result == 0) return true;

    if (p.revents & (POLLERR | POLLHUP | POLLRDHUP))
      return false;
    return true;
  }
};

#endif // NW_CONNECTION_MONITOR_H
