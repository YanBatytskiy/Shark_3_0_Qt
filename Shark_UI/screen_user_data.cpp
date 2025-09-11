#include "screen_user_data.h"
#include "ui_screen_user_data.h"

ScreenUserData::ScreenUserData(QWidget *parent) : QWidget(parent), ui(new Ui::ScreenUserData) { ui->setupUi(this); }

ScreenUserData::~ScreenUserData() { delete ui; }
