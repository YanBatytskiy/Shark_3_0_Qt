#include "chooseLogRegScreen.h"
#include "ui_chooseLogRegScreen.h"

chooseLogRegScreen::chooseLogRegScreen(std::shared_ptr<ClientSession> sessionPtr, QWidget *parent)
    : QDialog(parent), ui(new Ui::chooseLogRegScreen), _sessionPtr(std::move(sessionPtr)){

  ui->setupUi(this);
  ui->loginPage->setDatabase(_sessionPtr);
  // ui->loginPage->setMonitor(_connectionMonitorPtr);

  ui->regPage->setDatabase(_sessionPtr);
  // ui->regPage->setMonitor(_connectionMonitorPtr);

  connect(ui->loginPage, &loginScreen::registrationRequested, this, &chooseLogRegScreen::setRegistrationForm);
  connect(ui->loginPage, &loginScreen::rejected, this, &chooseLogRegScreen::onRejectedRequested);

  connect(ui->regPage, &registerScreen::loginRequested, this, &chooseLogRegScreen::setLoginForm);
  connect(ui->regPage, &registerScreen::rejected, this, &chooseLogRegScreen::onRejectedRequested);
}

chooseLogRegScreen::~chooseLogRegScreen() { delete ui; }

void chooseLogRegScreen::setLoginForm() { ui->stackedWidget->setCurrentIndex(0); }

void chooseLogRegScreen::setRegistrationForm() { ui->stackedWidget->setCurrentIndex(1); }

void chooseLogRegScreen::onRejectedRequested() {
  if (_sessionPtr) {
    _sessionPtr->stopConnectionThread(); // ← остановка фонового соединения
  }
  reject(); }
