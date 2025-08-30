#include "chooseLogRegScreen.h"
#include "ui_chooseLogRegScreen.h"

chooseLogRegScreen::chooseLogRegScreen(std::shared_ptr<Qt_Session> sessionPtr,QWidget *parent) : QDialog(parent), ui(new Ui::chooseLogRegScreen) {

  ui->setupUi(this);

  connect(ui->loginPage, &loginScreen::registrationRequested, this,&chooseLogRegScreen::setRegistrationForm);
  connect(ui->regPage, &registerScreen::loginRequested, this,&chooseLogRegScreen::setRegistrationForm);

}

chooseLogRegScreen::~chooseLogRegScreen() { delete ui; }

void chooseLogRegScreen::setLoginForm()
{
  ui->stackedWidget->setCurrentIndex(0);
}

void chooseLogRegScreen::setRegistrationForm()
{
  ui->stackedWidget->setCurrentIndex(1);

}
