#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "client_session.h"
#include "logger.h"
#include <QWidget>

class ConnectionMonitor;

namespace Ui {
class ScreenLogin;
}

class ScreenLogin : public QWidget {
  Q_OBJECT
public:
  explicit ScreenLogin(QWidget *parent = nullptr);
  ~ScreenLogin();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr, std::shared_ptr<Logger> loggerPtr);
  void clearFields();

signals:
  void registrationRequested();
  void accepted(QString login);
  void rejected();
  // void signalClearLogFile();

public slots:
  void onConnectionStatusChanged(bool connectionStatus,
                                 ServerConnectionMode mode);

private slots:
  void on_registerModeButton_clicked();
  void on_loginButtonBox_accepted();
  void on_loginButtonBox_rejected();

  void checkSignIn();

  void on_loginEdit_returnPressed();
  void on_passwordEdit_returnPressed();

  void on_baseReInitialisationPushButton_clicked();

  void slotOn_logFileClearPushButton_clicked();

private:
  Ui::ScreenLogin *ui;
  std::shared_ptr<ClientSession> _sessionPtr;
  std::shared_ptr<Logger> _loggerPtr;
  QString _systemData;
};

#endif // LOGINSCREEN_H
