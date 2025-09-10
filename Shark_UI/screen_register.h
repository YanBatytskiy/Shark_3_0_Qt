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

public slots:
  void onConnectionStatusChanged(bool connectionStatus,
                                 ServerConnectionMode mode);

private slots:
  void on_toLoginButton_clicked();
  void on_registerButtonBox_accepted();
  void on_registerButtonBox_rejected();

private:
  Ui::ScreenRegister *ui;
  std::shared_ptr<ClientSession> _sessionPtr;
};

#endif // REGISTERSCREEN_H
