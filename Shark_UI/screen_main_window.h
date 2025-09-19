#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "client_session.h"
#include <QMainWindow>
#include <memory>

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

  void setLoginForm();
  void setRegistrationForm();
  void setworkForm();

signals:

public slots:
  void onLoggedIn(QString login);
  void slotOnLogOut();
  void slotonRejectedRequested();

private slots:

private:
  Ui::MainWindow *ui;
  std::shared_ptr<ClientSession> _sessionPtr;

};
#endif // MAINWINDOW_H
