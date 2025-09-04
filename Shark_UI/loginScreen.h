#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "client_session.h"
#include <QDialog>

class ConnectionMonitor;

namespace Ui {
class loginScreen;
}

class loginScreen : public QDialog {
  Q_OBJECT
public:
  explicit loginScreen(QWidget *parent = nullptr);
  ~loginScreen();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);

signals:
  void registrationRequested();
  void rejected();

public slots:
  void onConnectionStatusChanged(bool connectionStatus, ServerConnectionMode mode);

private slots:
  void on_registerModeButton_clicked();
  void on_loginButtonBox_accepted();
  void on_loginButtonBox_rejected();

private:
  Ui::loginScreen *ui;
  std::shared_ptr<ClientSession> _sessionPtr;
};

#endif // LOGINSCREEN_H
