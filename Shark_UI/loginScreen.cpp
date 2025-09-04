#include "loginScreen.h"
#include "ui_loginScreen.h"
#include <QMessageBox>
#include "errorbus.h"
#include "exception_network.h"
#include "nw_connection_monitor.h"

loginScreen::loginScreen(QWidget *parent) : QDialog(parent), ui(new Ui::loginScreen) { ui->setupUi(this); }

loginScreen::~loginScreen() { delete ui; }

void loginScreen::setDatabase(std::shared_ptr<ClientSession> sessionPtr) { _sessionPtr = sessionPtr; }

void loginScreen::onConnectionStatusChanged(bool connectionStatus, ServerConnectionMode mode)
{

  // QString serverDataText;

  // if (mode == ServerConnectionMode::Localhost) serverDataText = "Подключено к серверу внутри рабочей станции.\n";
  // else if (mode == ServerConnectionMode::LocalNetwork) serverDataText = "Подключено к серверу внути локальной сети.\n";
  // else if (mode == ServerConnectionMode::Offline) serverDataText = "Подключение к серверу не удалось. Режим Offline.\n";


  if (connectionStatus){
    ui->serverStatusLabelRound->setStyleSheet("background-color: green; border-radius: 8px;");
    ui->serverStatusLabel->setText("server online");
  }
  else {
    ui->serverStatusLabelRound->setStyleSheet("background-color: red; border-radius: 8px;");
    ui->serverStatusLabel->setText("server offline");
  }
}

void loginScreen::on_registerModeButton_clicked() {

  ui->loginEdit->clear();
  ui->passwordEdit->clear();

  emit registrationRequested(); }

void loginScreen::on_loginButtonBox_accepted() {

  try{

    if (ui->loginEdit->text().toStdString() == "" || ui->passwordEdit->text().toStdString() == "")
    return;

  auto result = _sessionPtr->checkLoginPsswordQt(ui->loginEdit->text().toStdString(),
                                                 ui->passwordEdit->text().toStdString());

  if (!result) {
emit exc_qt::ErrorBus::i().error(tr("Login or Password is wrong"), "login");    return;

  }
    QMessageBox::information(this, tr("Sucsess"), tr("ok!!!"));
    _sessionPtr->registerClientOnDeviceQt(ui->loginEdit->text().toStdString());
    return;
  }
    catch (const std::exception&) {
    return;
  }
}

void loginScreen::on_loginButtonBox_rejected() { emit rejected(); }
