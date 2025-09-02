#include "loginScreen.h"
#include "ui_loginScreen.h"
#include <QMessageBox>
#include "errorbus.h"
#include "exception_network.h"

loginScreen::loginScreen(QWidget *parent) : QDialog(parent), ui(new Ui::loginScreen) {
  ui->setupUi(this);
}

loginScreen::~loginScreen() { delete ui; }

void loginScreen::setDatabase(std::shared_ptr<QtSession> sessionPtr) { _sessionPtr = sessionPtr; }

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

      // } catch (const exc_qt::NetworkException& ex) {
      // emit exc_qt::ErrorBus::i().error(QString::fromStdString(ex.what()), "network");
    return;
  }
}

void loginScreen::on_loginButtonBox_rejected() { emit rejected(); }
