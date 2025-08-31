#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "qt_session/qt_session.h"
#include <QDialog>

namespace Ui {
class loginScreen;
}

class loginScreen : public QDialog {
  Q_OBJECT
public:
  explicit loginScreen(QWidget *parent = nullptr);
  ~loginScreen();
  void setDatabase(std::shared_ptr<QtSession> sessionPtr);

signals:
  void registrationRequested();
  void rejected();

private slots:
  void on_registerModeButton_clicked();
  void on_loginButtonBox_accepted();
  void on_loginButtonBox_rejected();

private:
  Ui::loginScreen *ui;
  std::shared_ptr<QtSession> _sessionPtr;
};

#endif // LOGINSCREEN_H
