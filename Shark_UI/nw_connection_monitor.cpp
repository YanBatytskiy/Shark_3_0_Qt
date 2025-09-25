#include "nw_connection_monitor.h"
#include <thread>
#include <chrono>
#include <unistd.h>

void ConnectionMonitor::run() {
  try {
    bool online = false;
    ServerConnectionMode mode = ServerConnectionMode::Offline;

    while (_run.load(std::memory_order_acquire)) {

      if (!online) {
        // OFFLINE: пробуем найти адрес и подключиться
        if (!_session->findServerAddress(_session->getserverConnectionConfigCl(),
                                        _session->getserverConnectionModeCl())) {
          // mode = ServerConnectionMode::Offline;
          std::this_thread::sleep_for(std::chrono::milliseconds(500)); // пауза между попытками
          continue;
        }

        mode = _session->getserverConnectionModeCl();
        const int fd = _session->createConnection(_session->getserverConnectionConfigCl(), mode);

        if (fd < 0) {
          // Ошибки подключения — ждать и повторить
          // ECONNREFUSED/ETIMEDOUT/ENETUNREACH — типично при старте сервера после клиента
          std::this_thread::sleep_for(std::chrono::milliseconds(500)); // пауза между попытками
          continue;
        }

        _session->setSocketFd(fd);
        online = true;
        emit connectionStateChanged(true, mode);
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // пауза между попытками
        continue;
      } // if !online

      // ONLINE: проверяем живость сокета
      const int fd = _session->getSocketFd();
      const bool alive = socketAlive(fd);

      if (!alive) {
        online = false;
        mode = ServerConnectionMode::Offline;
        if (fd >= 0)
          ::close(fd);
        _session->setSocketFd(-1);
        emit connectionStateChanged(false, mode);

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // пауза между попытками
        continue;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(500)); // пауза между попытками
    } // while _run.load
  } catch (const std::exception&) {
    return;
  }
}
