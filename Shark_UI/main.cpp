#include "mainwindow.h"
#include <QApplication>
#include "qt_session/qt_session.h"
#include <memory>
#include "client.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
app.setStyle(QStyleFactory::create("Fusion"));
  std::shared_ptr<QtSession> sessionPtr(&qtSession, [](QtSession*) {});

  auto w = MainWindow::createSession(sessionPtr);

  if (w)
    w->show();
  else
    return 0;
  return app.exec();
}
