#include "screen_register.h"
#include "ui_screen_register.h"

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
