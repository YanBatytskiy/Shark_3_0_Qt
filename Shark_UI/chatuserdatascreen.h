#ifndef CHATUSERDATASCREEN_H
#define CHATUSERDATASCREEN_H

#include <QDialog>

namespace Ui {
class chatListScreen;
}

class chatListScreen : public QDialog {
  Q_OBJECT

public:
  explicit chatListScreen(QWidget *parent = nullptr);
  ~chatListScreen();

private:
  Ui::chatListScreen *ui;
};

#endif // CHATUSERDATASCREEN_H
