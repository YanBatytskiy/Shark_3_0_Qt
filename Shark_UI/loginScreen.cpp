#include "loginScreen.h"
#include "ui_loginScreen.h"
#include <QMessageBox>

loginScreen::loginScreen(QWidget *parent) : QDialog(parent), ui(new Ui::loginScreen) { ui->setupUi(this); }

loginScreen::~loginScreen() { delete ui; }

void loginScreen::setDatabase(std::shared_ptr<QtSession> sessionPtr) { _sessionPtr = sessionPtr; }

void loginScreen::on_registerModeButton_clicked() { emit registrationRequested(); }

void loginScreen::on_loginButtonBox_accepted() {

  if (ui->loginEdit->text().toStdString() == "" || ui->passwordEdit->text().toStdString() == "")
    return;

  auto result = _sessionPtr->checkLoginPsswordQt(ui->loginEdit->text().toStdString(),
                                                 ui->passwordEdit->text().toStdString());

  if (!result) {
    QMessageBox::critical(this, tr("Err"), tr("Login or Password is wrong"));
    return;

  } else {
    QMessageBox::information(this, tr("Sucsess"), tr("ok!!!"));
    result = _sessionPtr->registerClientOnDeviceQt(ui->loginEdit->text().toStdString());
    return;
  }
}

void loginScreen::on_loginButtonBox_rejected() { emit rejected(); }
