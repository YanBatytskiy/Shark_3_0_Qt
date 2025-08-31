#ifndef REGISTERSCREEN_H
#define REGISTERSCREEN_H

#include "qt_session/qt_session.h"
#include <QDialog>

namespace Ui {
class registerScreen;
}

class registerScreen : public QDialog {
  Q_OBJECT

public:
  explicit registerScreen(QWidget *parent = nullptr);
  ~registerScreen();
  void setDatabase(std::shared_ptr<QtSession> sessionPtr);

signals:
  void loginRequested();
  void rejected();

private slots:
  void on_toLoginButton_clicked();
  void on_registerButtonBox_accepted();
  void on_registerButtonBox_rejected();

private:
  Ui::registerScreen *ui;
  std::shared_ptr<QtSession> _sessionPtr;
};

#endif // REGISTERSCREEN_H
