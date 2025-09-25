#include "screen_main_window.h"
#include "logger.h"
#include "screen_login.h"
#include "ui_screen_main_window.h"

#include <QTimeZone>
#include <QPushButton>

MainWindow::MainWindow(std::shared_ptr<ClientSession> sessionPtr,
                       std::shared_ptr<Logger> loggerPtr,
                       QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  _sessionPtr = std::move(sessionPtr);
  _loggerPtr = std::move(loggerPtr);

  ui->setupUi(this);

  ui->pageLogin->setDatabase(_sessionPtr, _loggerPtr);
  ui->pageRegister->setDatabase(_sessionPtr, _loggerPtr);
  ui->pageWork->setDatabase(_sessionPtr, _loggerPtr);

  connect(ui->pageLogin, &ScreenLogin::registrationRequested, this,
          &MainWindow::setRegistrationForm);

  connect(ui->pageLogin, &ScreenLogin::rejected, this,
          &MainWindow::slotonRejectedRequested);

  connect(ui->pageLogin, &ScreenLogin::accepted, this, &MainWindow::onLoggedIn);

  connect(ui->pageRegister, &ScreenRegister::loginRequested, this,
          &MainWindow::setLoginForm);

  connect(ui->pageRegister, &ScreenRegister::signalLoggedIn, this,
          &MainWindow::onLoggedIn);

  connect(ui->pageRegister, &ScreenRegister::rejected, this,
          &MainWindow::slotonRejectedRequested);

  if (auto w = ui->pageWork) {
    connect(ui->pageWork, &ScreenMainWork::signalLogOut, this, &MainWindow::slotOnLogOut);

    connect(ui->pageWork, &ScreenMainWork::signalShowProfile, this, &MainWindow::slotShowProfile);
  }

  auto profile = ui->mainWindowstackedWidget->findChild<ScreenUserProfile *>("pageProfile");

  connect(profile, &ScreenUserProfile::signalCloseUserProfile, this, &MainWindow::setworkForm);

  profile->setDatabase(_sessionPtr);

  setLoginForm();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setLoginForm() {
  ui->pageLogin->clearFields();
  QWidget *page =
      ui->mainWindowstackedWidget->findChild<QWidget *>("pageLogin");

  ui->mainWindowstackedWidget->setCurrentWidget(page);
}

void MainWindow::setRegistrationForm() {
  ui->pageRegister->clearFields();
  QWidget *page = ui->mainWindowstackedWidget->findChild<QWidget *>("pageRegister");

  ui->mainWindowstackedWidget->setCurrentWidget(page);
}

void MainWindow::setworkForm() {
  QWidget *page = ui->mainWindowstackedWidget->findChild<QWidget *>("pageWork");

  ui->mainWindowstackedWidget->setCurrentWidget(page);
}

void MainWindow::onLoggedIn(QString login) {
  _sessionPtr->registerClientOnDeviceQt(login.toStdString());
  ui->pageWork->createSession();
  setworkForm();
}

void MainWindow::slotOnLogOut() {

  _sessionPtr->resetSessionData();
  setLoginForm();
}

void MainWindow::slotonRejectedRequested() {
  if (_sessionPtr) {
    _sessionPtr->stopConnectionThread(); // ← остановка фонового соединения
  }
  close();
}

void MainWindow::slotShowProfile()
{
  QWidget *page = ui->mainWindowstackedWidget->findChild<QWidget* >("pageProfile");
  ui->mainWindowstackedWidget->setCurrentWidget(page);

  auto w = ui->mainWindowstackedWidget->findChild<ScreenUserProfile*>("pageProfile");
  auto b = w->findChild<QPushButton* >("savePushButton");
  b->setEnabled(false);

  b = w->findChild<QPushButton* >("changePasswordPushButton");
  b->setEnabled(false);

  w->slotFillDataToForm();
}

void MainWindow::slotCloseProfile()
{
  QWidget *page = ui->mainWindowstackedWidget->findChild<QWidget* >("pageWork");
  ui->mainWindowstackedWidget->setCurrentWidget(page);

}
