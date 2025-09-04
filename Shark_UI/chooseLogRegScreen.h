#ifndef CHOOSELOGREGSCREEN_H
#define CHOOSELOGREGSCREEN_H

#include "client_session.h"
#include <QDialog>

namespace Ui {
class chooseLogRegScreen;
}

class chooseLogRegScreen : public QDialog {
  Q_OBJECT

public:
  explicit chooseLogRegScreen(std::shared_ptr<ClientSession> sessionPtr = nullptr, QWidget *parent = nullptr);
  ~chooseLogRegScreen();

  void setLoginForm();
  void setRegistrationForm();

public slots:
  void onRejectedRequested();

private:
  Ui::chooseLogRegScreen *ui;
  std::shared_ptr<ClientSession> _sessionPtr;
};

#endif // CHOOSELOGREGSCREEN_H
