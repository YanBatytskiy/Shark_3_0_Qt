#include "chat_system/chat_system.h"
#include "client/client_session.h"
#include "errorbus.h"
#include "logger.h"
#include "screen_main_window.h"
#include <QApplication>
#include <QMessageBox>
#include <QObject>
#include <QStyleFactory>
#include <arpa/inet.h>
#include <memory>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  ChatSystem clientSystem;
  auto sessionPtr = std::make_shared<ClientSession>(clientSystem);
  auto loggerPtr = std::make_shared<Logger>();

  (void)exc_qt::ErrorBus::i();

  QObject::connect(&exc_qt::ErrorBus::i(), &exc_qt::ErrorBus::error, &app,
                   [loggerPtr](const QString &m, const QString &ctx) {
                     QMessageBox::critical(qApp->activeWindow(), "Ошибка",
                                           QString("[%1]\n%2").arg(ctx, m));

                     // записать в лог
                     QString log_line = QStringLiteral("%1   %2")
                                            .arg(ctx, m);
                     emit loggerPtr->signalWriteLine(log_line);
                   });

  sessionPtr->startConnectionThread();

  QObject::connect(&app, &QCoreApplication::aboutToQuit,
                   [&] { sessionPtr->stopConnectionThread(); });

  QObject::connect(&app, &QCoreApplication::aboutToQuit,
                   loggerPtr.get(), &Logger::slotStopLogger);

  auto w = new MainWindow(sessionPtr, loggerPtr);
  w->show();
  auto result = app.exec();

  if (result == QDialog::Rejected)
    return 0;

  return 0;
}
