#ifndef CHATUSERDATASCREEN_H
#define CHATUSERDATASCREEN_H

#include <QWidget>

namespace Ui {
class chatListScreen;
}

class chatListScreen : public QWidget {
  Q_OBJECT

public:
  explicit chatListScreen(QWidget *parent = nullptr);
  ~chatListScreen();

private:
  Ui::chatListScreen *ui;
};

#endif // CHATUSERDATASCREEN_H
