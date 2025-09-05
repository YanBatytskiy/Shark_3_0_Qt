#include "loginScreen.h"
#include "ui_loginScreen.h"
#include <QMessageBox>
#include "errorbus.h"
#include <sys/utsname.h>

loginScreen::loginScreen(QWidget *parent) : QDialog(parent), ui(new Ui::loginScreen) { ui->setupUi(this);
}

loginScreen::~loginScreen() { delete ui; }

void loginScreen::setDatabase(std::shared_ptr<ClientSession> sessionPtr) {

  _sessionPtr = sessionPtr;

  connect(_sessionPtr.get(), &ClientSession::serverStatusChanged, this, &loginScreen::onConnectionStatusChanged, Qt::QueuedConnection);

  struct utsname utsname;
  uname(&utsname);

  _systemData = "OS name: " + QString::fromStdString(utsname.sysname) +
                       "\nOS Release: " + QString::fromStdString(utsname.release) +
                       "\nOS version: " + QString::fromStdString(utsname.version) +
                       "\nArchitecture: " + QString::fromStdString(utsname.machine) +
                       "\nHost name: " + QString::fromStdString(utsname.nodename);

  QString serverDataText;

  serverDataText = _systemData;

  auto mode = _sessionPtr->getserverConnectionModeCl();

  if (mode == ServerConnectionMode::Localhost) serverDataText += "\n\nПодключено к серверу внутри рабочей станции.";
  else if (mode == ServerConnectionMode::LocalNetwork) serverDataText += "\n\nПодключено к серверу внути локальной сети.";
  else if (mode == ServerConnectionMode::Offline)
    serverDataText += "\n\nПодключение к серверу не удалось. Режим Offline.";

  ServerConnectionConfig serverConnectionConfig;

  if (mode == ServerConnectionMode::Localhost) {
    serverDataText += QString::fromStdString("\nLocalHost address: ");
    serverDataText += QString::fromStdString(serverConnectionConfig.addressLocalHost);
    serverDataText += QString::fromStdString("\nPort: ");
    serverDataText += QString::number(serverConnectionConfig.port);
  }
  else if (mode == ServerConnectionMode::LocalNetwork) {
    serverDataText += QString::fromStdString("\nLocal Network Address: ");
    serverDataText += QString::fromStdString(serverConnectionConfig.addressLocalNetwork);
    serverDataText += QString::fromStdString("\nPort: ");
    serverDataText += QString::number(serverConnectionConfig.port);
  };

  ui->serverDataLabel->setText(serverDataText);

}

void loginScreen::onConnectionStatusChanged(bool connectionStatus, ServerConnectionMode mode)
{

  QString serverDataText;

  serverDataText = _systemData;

  if (mode == ServerConnectionMode::Localhost) serverDataText += "\n\nПодключено к серверу внутри рабочей станции.";
  else if (mode == ServerConnectionMode::LocalNetwork) serverDataText += "\n\nПодключено к серверу внути локальной сети.";
  else if (mode == ServerConnectionMode::Offline)
    serverDataText += "\n\nПодключение к серверу не удалось. Режим Offline.";

  ServerConnectionConfig serverConnectionConfig;

  if (mode == ServerConnectionMode::Localhost) {
    serverDataText += QString::fromStdString("\nLocalHost address: ");
    serverDataText += QString::fromStdString(serverConnectionConfig.addressLocalHost);
    serverDataText += QString::fromStdString("\nPort: ");
    serverDataText += QString::number(serverConnectionConfig.port);
  }
  else if (mode == ServerConnectionMode::LocalNetwork) {
    serverDataText += QString::fromStdString("\nLocal Network Address: ");
    serverDataText += QString::fromStdString(serverConnectionConfig.addressLocalNetwork);
    serverDataText += QString::fromStdString("\nPort: ");
    serverDataText += QString::number(serverConnectionConfig.port);
  };


  ui->serverDataLabel->setText(serverDataText);

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

  if (!_sessionPtr->getIsServerOnline()) {
    QMessageBox::warning(this, tr("No!"), tr("Сервер не доступен."));
  }
  else {
    ui->loginEdit->clear();
    ui->passwordEdit->clear();

    emit registrationRequested();
  }
}

void loginScreen::on_loginButtonBox_accepted() {

  try{

    if (ui->loginEdit->text().toStdString() == "" || ui->passwordEdit->text().toStdString() == "")
    return;

    if (!_sessionPtr->getIsServerOnline()) {
      QMessageBox::warning(this, tr("No!"), tr("Сервер не доступен."));
      return;
    }


  auto result = _sessionPtr->checkLoginPsswordQt(ui->loginEdit->text().toStdString(),
                                                 ui->passwordEdit->text().toStdString());

  if (!result) {
emit exc_qt::ErrorBus::i().error(tr("Login or Password is wrong"), "login");    return;

  }
    // QMessageBox::information(this, tr("Sucsess"), tr("ok!!!"));
    // return;

  emit accepted(ui->loginEdit->text());

  }
    catch (const std::exception&) {
    return;
  }
}

void loginScreen::on_loginButtonBox_rejected() { emit rejected(); }
