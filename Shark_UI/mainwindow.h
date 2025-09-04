#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "client_session.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(std::shared_ptr<ClientSession> sessionPtr, QWidget *parent = nullptr);
  ~MainWindow();
  static MainWindow *createSession(std::shared_ptr<ClientSession> sessionPtr);
  static int kInstanceCount;

signals:
  void systemDataChanged(const QString &data);


private:
  Ui::MainWindow *ui;
  std::shared_ptr<ClientSession> _sessionPtr;

};
#endif // MAINWINDOW_H
