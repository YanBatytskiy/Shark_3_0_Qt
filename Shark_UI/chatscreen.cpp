#include "chatscreen.h"
#include "ui_chatscreen.h"

chatScreen::chatScreen(QWidget *parent) : QDialog(parent), ui(new Ui::chatScreen) { ui->setupUi(this); }

chatScreen::~chatScreen() { delete ui; }
