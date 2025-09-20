#include "screen_register.h"
#include "ui_screen_register.h"
#include <QMessageBox>

ScreenRegister::ScreenRegister(QWidget *parent)
    : QWidget(parent), ui(new Ui::ScreenRegister) {
  ui->setupUi(this);
}

ScreenRegister::~ScreenRegister() { delete ui; }

void ScreenRegister::setDatabase(std::shared_ptr<ClientSession> sessionPtr) {
  _sessionPtr = sessionPtr;
  connect(_sessionPtr.get(), &ClientSession::serverStatusChanged, this,
          &ScreenRegister::onConnectionStatusChanged, Qt::QueuedConnection);
}

void ScreenRegister::clearFields() {
  ui->loginEdit->clear();
  ui->nameEdit->clear();
  ui->passwordEdit->clear();
  ui->passwordConfirmEdit->clear();
  ui->loginEdit->setFocus();
}

void ScreenRegister::onConnectionStatusChanged(bool connectionStatus,
                                               ServerConnectionMode mode) {

  if (connectionStatus) {
    ui->serverStatusLabelRound->setStyleSheet(
        "background-color: green; border-radius: 8px;");
    ui->serverStatusLabel->setText("server online");
  } else {
    ui->serverStatusLabelRound->setStyleSheet(
        "background-color: red; border-radius: 8px;");
    ui->serverStatusLabel->setText("server offline");
  }
}

void ScreenRegister::on_toLoginButton_clicked() {
  ui->loginEdit->clear();
  ui->nameEdit->clear();
  ui->passwordEdit->clear();
  ui->passwordConfirmEdit->clear();
  emit loginRequested();
}

void ScreenRegister::on_registerButtonBox_accepted() {}

void ScreenRegister::on_registerButtonBox_rejected() {
  clearFields();
  emit rejected();
}

void ScreenRegister::on_loginEdit_editingFinished()
{
  if (!_sessionPtr->inputNewLoginValidationQt(ui->loginEdit->text().toStdString(),5,20)) {
    QMessageBox::critical(this,"Ошибка", "Недопустимый логин. Наведи мышку на поле для полсказки.");
    ui->loginEdit->setStyleSheet("QLineEdit { color: red; }");
  }
  ui->loginEdit->setStyleSheet("QLineEdit { color: black; }");
}


void ScreenRegister::on_nameEdit_editingFinished()
{
  if (!_sessionPtr->inputNewLoginValidationQt(ui->nameEdit->text().toStdString(),2,20)) {
    QMessageBox::critical(this,"Ошибка", "Недопустимое имя. Наведи мышку на поле для полсказки.");
    ui->nameEdit->setStyleSheet("QLineEdit { color: red; }");
  }
  ui->nameEdit->setStyleSheet("QLineEdit { color: black; }");

}


void ScreenRegister::on_passwordEdit_editingFinished() {
  if (!_sessionPtr->inputNewPasswordValidationQt(ui->passwordEdit->text().toStdString(), 5, 20)) {
    QMessageBox::critical(this, "Ошибка", "Недопустимый пароль. Наведи мышку на поле для полсказки.");
      ui->passwordEdit->setStyleSheet("QLineEdit { color: red; }");
  }
ui->passwordEdit->setStyleSheet("QLineEdit { color: black; }");

}

