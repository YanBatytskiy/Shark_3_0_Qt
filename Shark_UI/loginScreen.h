#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QDialog>
#include "qt_session/qt_session.h"


namespace Ui { class loginScreen; }

class loginScreen : public QDialog {
  Q_OBJECT
public:
  explicit loginScreen(QWidget *parent = nullptr);
  ~loginScreen();
  void setDatabase(  std::shared_ptr<Qt_Session> sessionPtr);

signals:
  void registrationRequested();

private slots:
  void on_registerModeButton_clicked();

  void on_loginButtonBox_accepted();

  void on_loginButtonBox_rejected();

private:
  Ui::loginScreen *ui;
  std::shared_ptr<Qt_Session> _sessionPtr;

};

#endif // LOGINSCREEN_H
