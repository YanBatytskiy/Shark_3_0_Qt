#include "nw_connection_monitor.h"
#include <thread>
#include <chrono>

void ConnectionMonitor::run() {
  try {
    const bool ok = _session->initServerConnection();
    emit connectionStateChanged(ok, _session->getserverConnectionModeCl());

    while (_run) {
      const int fd = _session->getSocketFd();
      const bool alive = socketAlive(fd);

      if (!alive) {
        _session->getInstance().setIsServerStatus(false);
        emit connectionStateChanged(false, _session->getserverConnectionModeCl());

        for (;;) {
          if (!_run) break;
          if (_session->findServerAddress(_session->getserverConnectionConfigCl(),
                                          _session->getserverConnectionModeCl())) {
            const int newFd = _session->createConnection(_session->getserverConnectionConfigCl(),
                                                         _session->getserverConnectionModeCl());
            if (newFd >= 0) {
              _session->getInstance().setIsServerStatus(true);
              emit connectionStateChanged(true, _session->getserverConnectionModeCl());
              break;
            }
          }
          std::this_thread::sleep_for(std::chrono::seconds(2)); // пауза между попытками
        }
      }

      std::this_thread::sleep_for(std::chrono::seconds(1));     // период опроса
    }
  } catch (const std::exception&) {
    return;
  }
}
