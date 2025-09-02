#include "chatuserdatascreen.h"
#include "ui_chatuserdatascreen.h"

chatListScreen::chatListScreen(QWidget *parent) : QDialog(parent), ui(new Ui::chatListScreen) { ui->setupUi(this); }

chatListScreen::~chatListScreen() { delete ui; }
