#include "registerScreen.h"
#include "ui_registerScreen.h"

registerScreen::registerScreen(QWidget *parent) : QDialog(parent), ui(new Ui::registerScreen) { ui->setupUi(this); }

registerScreen::~registerScreen() { delete ui; }

void registerScreen::setDatabase(std::shared_ptr<QtSession> sessionPtr) { _sessionPtr = sessionPtr; }

void registerScreen::on_toLoginButton_clicked()
{
  emit loginRequested();
}


void registerScreen::on_registerButtonBox_accepted()
{

}


void registerScreen::on_registerButtonBox_rejected()
{
  emit rejected();

}

