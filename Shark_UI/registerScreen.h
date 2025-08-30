#ifndef REGISTERSCREEN_H
#define REGISTERSCREEN_H

#include <QDialog>

namespace Ui {
class registerScreen;
}

class registerScreen : public QDialog {
  Q_OBJECT

public:
  explicit registerScreen(QWidget *parent = nullptr);
  ~registerScreen();

signals:
  void loginRequested();

private slots:
  void on_toLoginButton_clicked();


  void on_registerButtonBox_accepted();

  void on_registerButtonBox_rejected();

private:
  Ui::registerScreen *ui;
};

#endif // REGISTERSCREEN_H
