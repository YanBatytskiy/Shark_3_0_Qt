#ifndef CHOOSELOGREGSCREEN_H
#define CHOOSELOGREGSCREEN_H

#include "qt_session/qt_session.h"
#include <QDialog>

namespace Ui {
class chooseLogRegScreen;
}

class chooseLogRegScreen : public QDialog {
  Q_OBJECT

public:
  explicit chooseLogRegScreen(std::shared_ptr<QtSession> sessionPtr = nullptr, QWidget *parent = nullptr);
  ~chooseLogRegScreen();

  void setLoginForm();
  void setRegistrationForm();

public slots:
  void onRejectedRequested();

private:
  Ui::chooseLogRegScreen *ui;
  std::shared_ptr<QtSession> _sessionPtr;
};

#endif // CHOOSELOGREGSCREEN_H
