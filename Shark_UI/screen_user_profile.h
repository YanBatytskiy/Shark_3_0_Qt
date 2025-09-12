#ifndef SCREEN_USER_PROFILE_H
#define SCREEN_USER_PROFILE_H

#include <QWidget>

namespace Ui {
class ScreenUserProfile;
}

class ScreenUserProfile : public QWidget {
  Q_OBJECT

public:
  explicit ScreenUserProfile(QWidget *parent = nullptr);
  ~ScreenUserProfile();

private:
  Ui::ScreenUserProfile *ui;
};

#endif // SCREEN_USER_PROFILE_H
