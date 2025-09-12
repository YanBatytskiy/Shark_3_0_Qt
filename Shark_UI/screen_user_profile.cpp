#include "screen_user_profile.h"
#include "ui_screen_user_profile.h"

ScreenUserProfile::ScreenUserProfile(QWidget *parent) : QWidget(parent), ui(new Ui::ScreenUserProfile) {
  ui->setupUi(this);
}

ScreenUserProfile::~ScreenUserProfile() { delete ui; }
