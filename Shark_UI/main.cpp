#include "mainwindow.h"
#include <QApplication>
#include <memory>
#include <QStyleFactory>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "chat_system/chat_system.h"
#include "client/client_session.h"
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
                     QMessageBox::critical(qApp->activeWindow(), "Ошибка",
                                           QString("[%1]\n%2").arg(ctx, m));
                   });


  ChatSystem  clientSystem;

  auto sessionPtr = std::make_shared<ClientSession>(clientSystem);

  sessionPtr->startConnectionThread();

  QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]{
    sessionPtr->stopConnectionThread();
  });

  auto w = MainWindow::createSession(sessionPtr); // или sessionPtr.get() — по сигнатуре

  if (!w) return 0;

  w->setAttribute(Qt::WA_DeleteOnClose);
  w->show();
  return app.exec();
}
