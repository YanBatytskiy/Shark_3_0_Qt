#include "registerScreen.h"
#include "ui_registerScreen.h"
// #include "nw_connection_monitor.h"


registerScreen::registerScreen(QWidget *parent) : QDialog(parent), ui(new Ui::registerScreen) { ui->setupUi(this); }

registerScreen::~registerScreen() { delete ui; }

void registerScreen::setDatabase(std::shared_ptr<ClientSession> sessionPtr) { _sessionPtr = sessionPtr;
  connect(_sessionPtr.get(), &ClientSession::serverStatusChanged, this, &registerScreen::onConnectionStatusChanged, Qt::QueuedConnection);
}

void registerScreen::onConnectionStatusChanged(bool connectionStatus, ServerConnectionMode mode)
{

  if (connectionStatus){
    ui->serverStatusLabelRound->setStyleSheet("background-color: green; border-radius: 8px;");
    ui->serverStatusLabel->setText("server online");
  }
  else {
    ui->serverStatusLabelRound->setStyleSheet("background-color: red; border-radius: 8px;");
    ui->serverStatusLabel->setText("server offline");
  }

}

void registerScreen::on_toLoginButton_clicked()
{
  ui->loginEdit->clear();
  ui->nameEdit->clear();
  ui->passwordEdit->clear();
  ui->passwordConfirmEdit->clear();
  emit loginRequested();
}


void registerScreen::on_registerButtonBox_accepted()
{

}


void registerScreen::on_registerButtonBox_rejected()
{
  emit rejected();

}

