#include "screen_main_window.h"
#include "screen_login.h"
#include "ui_screen_main_window.h"

#include <QTimeZone>

MainWindow::MainWindow(std::shared_ptr<ClientSession> sessionPtr,
                       QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  _sessionPtr = std::move(sessionPtr);

  ui->setupUi(this);

  ui->pageLogin->setDatabase(_sessionPtr);
  ui->pageRegister->setDatabase(_sessionPtr);
  ui->pageWork->setDatabase(_sessionPtr);

  connect(ui->pageLogin, &ScreenLogin::registrationRequested, this,
          &MainWindow::setRegistrationForm);
  connect(ui->pageLogin, &ScreenLogin::rejected, this,
          &MainWindow::slotonRejectedRequested);
  connect(ui->pageLogin, &ScreenLogin::accepted, this, &MainWindow::onLoggedIn);

  connect(ui->pageRegister, &ScreenRegister::loginRequested, this,
          &MainWindow::setLoginForm);
  connect(ui->pageRegister, &ScreenRegister::rejected, this,
          &MainWindow::slotonRejectedRequested);
  if (auto w = ui->pageWork)

    connect(ui->pageWork, &ScreenMainWork::signalLogOut, this, &MainWindow::slotOnLogOut);

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
