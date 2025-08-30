#ifndef CHOOSELOGREGSCREEN_H
#define CHOOSELOGREGSCREEN_H

#include <QDialog>
#include "qt_session/qt_session.h"

namespace Ui {
class chooseLogRegScreen;
}

class chooseLogRegScreen : public QDialog {
  Q_OBJECT

public:
  explicit chooseLogRegScreen(std::shared_ptr<Qt_Session> sessionPtr = nullptr, QWidget *parent = nullptr);
  ~chooseLogRegScreen();

  void setLoginForm();
  void setRegistrationForm();

private:
  Ui::chooseLogRegScreen *ui;
  std::shared_ptr<Qt_Session> _sessionPtr;
};

#endif // CHOOSELOGREGSCREEN_H
