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

  void setLoginForm();
  void setRegistrationForm();
  void setworkForm();


public slots:
  void onRejectedRequested();
  void onLoggedIn(QString login);

private slots:
  void on_exitAction_triggered();


  void on_mainWindowstackedWidget_currentChanged(int arg1);

private:
  Ui::MainWindow *ui;
  std::shared_ptr<ClientSession> _sessionPtr;

};
#endif // MAINWINDOW_H
