#include "nw_connection_monitor.h"
#include <thread>
#include <chrono>

void ConnectionMonitor::run() {
  try{
    // первичный старт/поиск
    _session->initServerConnection();
    emit connectionStateChanged(_session->getInstance().getIsServerStatus(), _session->getserverConnectionModeCl());

    while (_run) {
      // проверка текущего состояния
      const int fd = _session->getSocketFd();
      const bool statusFd = socketAlive(fd);

      if (!statusFd) {
        // оффлайн → попытки восстановить
        _session->getInstance().setIsServerStatus(false);
        emit connectionStateChanged(false, _session->getserverConnectionModeCl());

        // цикл реконнекта
        for (;;) {
          if (!_run)
            break;
          if (_session->findServerAddress(_session->getserverConnectionConfigCl(), _session->getserverConnectionModeCl())){
            int newFd = _session->createConnection(_session->getserverConnectionConfigCl(),
                                                   _session->getserverConnectionModeCl());

            if (newFd >=0) {
              _session->getInstance().setIsServerStatus(true);
              emit connectionStateChanged(true, _session->getserverConnectionModeCl());
              break;
            } // if newFd
          }// if findServer
        } // for
        std::this_thread::sleep_for(std::chrono::seconds(2));

      } //if !statusFd

    } //while run
    std::this_thread::sleep_for(std::chrono::seconds(1));

  }
  catch (const std::exception&) {
    return;
  }
}
