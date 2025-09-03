#include "mainwindow.h"
#include <QApplication>
#include <memory>
#include <QStyleFactory>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "chat_system/chat_system.h"
#include "client/client_session.h"
#include "qt_session/qt_session.h"
#include "nw_connection_monitor.h"
#include "errorbus.h"
#include <QMessageBox>
#include <QObject>


int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  (void)exc_qt::ErrorBus::i();

  QObject::connect(&exc_qt::ErrorBus::i(), &exc_qt::ErrorBus::error,
                   &app,
                   [](const QString& m, const QString& ctx){
                     QMessageBox::critical(nullptr, "Ошибка",
                                           QString("[%1]\n%2").arg(ctx, m));
                   });


  ChatSystem  clientSystem;
  // ClientSession clientSession(clientSystem);

  auto sessionPtr = std::make_shared<QtSession>(clientSystem);

  ConnectionMonitor connectionMonitor(&sessionPtr->getQtClientSession());


  auto w = MainWindow::createSession(sessionPtr);

  if (w)
    w->show();
  else
    return 0;

  close(sessionPtr->getQtClientSession().getSocketFd());
  return app.exec();



}
