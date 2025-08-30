#ifndef CHATSCREEN_H
#define CHATSCREEN_H

#include <QDialog>

namespace Ui {
class chatScreen;
}

class chatScreen : public QDialog {
  Q_OBJECT

public:
  explicit chatScreen(QWidget *parent = nullptr);
  ~chatScreen();

private:
  Ui::chatScreen *ui;
};

#endif // CHATSCREEN_H
