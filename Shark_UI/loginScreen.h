#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "client_session.h"
#include <QWidget>

class ConnectionMonitor;

namespace Ui {
class loginScreen;
}

class loginScreen : public QWidget {
  Q_OBJECT
public:
  explicit loginScreen(QWidget *parent = nullptr);
  ~loginScreen();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);
  void clearFields();

signals:
  void registrationRequested();
  void accepted(QString login);
  void rejected();

public slots:
  void onConnectionStatusChanged(bool connectionStatus, ServerConnectionMode mode);

private slots:
  void on_registerModeButton_clicked();
  void on_loginButtonBox_accepted();
  void on_loginButtonBox_rejected();

  void checkSignIn();

  void on_loginEdit_returnPressed();
  void on_passwordEdit_returnPressed();

private:
  Ui::loginScreen *ui;
  std::shared_ptr<ClientSession> _sessionPtr;
  QString _systemData;
};

#endif // LOGINSCREEN_H
