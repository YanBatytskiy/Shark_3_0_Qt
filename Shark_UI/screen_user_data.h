#ifndef SCREEN_USER_DATA_H
#define SCREEN_USER_DATA_H

#include <QWidget>

namespace Ui {
class ScreenUserData;
}

class ScreenUserData : public QWidget {
  Q_OBJECT

public:
  explicit ScreenUserData(QWidget *parent = nullptr);
  ~ScreenUserData();

private:
  Ui::ScreenUserData *ui;
};

#endif // SCREEN_USER_DATA_H
