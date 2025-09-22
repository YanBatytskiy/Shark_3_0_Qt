#ifndef REGISTERSCREEN_H
#define REGISTERSCREEN_H

#include "client_session.h"

#include <QWidget>

class ConnectionMonitor;

namespace Ui {
class ScreenRegister;
}

class ScreenRegister : public QWidget {
  Q_OBJECT

public:
  explicit ScreenRegister(QWidget *parent = nullptr);
  ~ScreenRegister();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);
  void clearFields();

signals:
  void loginRequested();
  void rejected();
  void signalLoggedIn(QString login);

public slots:
  void onConnectionStatusChanged(bool connectionStatus,
                                 ServerConnectionMode mode);

private slots:
  void on_toLoginButton_clicked();

  void on_loginEdit_editingFinished();

  void on_nameEdit_editingFinished();

  void on_passwordEdit_editingFinished();

  void on_passwordConfirmEdit_editingFinished();

  void on_exitPushButton_clicked();

  void on_registerPushButton_clicked();

private:
  Ui::ScreenRegister *ui;
  std::shared_ptr<ClientSession> _sessionPtr;
  bool isLogin{false};
  bool isName{false};
  bool isPassword{false};
};

#endif // REGISTERSCREEN_H
