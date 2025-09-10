#include "screen_main_window.h"
#include "screen_login.h"
#include "ui_screen_main_window.h"

#include <QTimeZone>

MainWindow::MainWindow(std::shared_ptr<ClientSession> sessionPtr,
                       QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  if (!sessionPtr) {
    qWarning() << "ClientSession is null";
    return;
  }
  _sessionPtr = std::move(sessionPtr);

  ui->setupUi(this);

  ui->loginPage->setDatabase(_sessionPtr);
  ui->regPage->setDatabase(_sessionPtr);
  ui->workPage->setDatabase(_sessionPtr);

  connect(ui->loginPage, &ScreenLogin::registrationRequested, this,
          &MainWindow::setRegistrationForm);
  connect(ui->loginPage, &ScreenLogin::rejected, this,
          &MainWindow::onRejectedRequested);
  connect(ui->loginPage, &ScreenLogin::accepted, this, &MainWindow::onLoggedIn);

  connect(ui->regPage, &ScreenRegister::loginRequested, this,
          &MainWindow::setLoginForm);
  connect(ui->regPage, &ScreenRegister::rejected, this,
          &MainWindow::onRejectedRequested);

  setLoginForm();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setLoginForm() {
  ui->loginPage->clearFields();
  QWidget *page =
      ui->mainWindowstackedWidget->findChild<QWidget *>("loginPage");
  ui->mainWindowstackedWidget->setCurrentWidget(page);
}

void MainWindow::setRegistrationForm() {
  ui->regPage->clearFields();
  QWidget *page = ui->mainWindowstackedWidget->findChild<QWidget *>("regPage");
  ui->mainWindowstackedWidget->setCurrentWidget(page);
}

void MainWindow::setworkForm() {
  QWidget *page = ui->mainWindowstackedWidget->findChild<QWidget *>("workPage");
  ui->mainWindowstackedWidget->setCurrentWidget(page);
}

void MainWindow::onRejectedRequested() {
  if (_sessionPtr) {
    _sessionPtr->stopConnectionThread(); // ← остановка фонового соединения
  }
  close();
}

void MainWindow::onLoggedIn(QString login) {
  _sessionPtr->registerClientOnDeviceQt(login.toStdString());
  ui->workPage->createSession();
  setworkForm();
}

void MainWindow::on_exitAction_triggered() {
  _sessionPtr->resetSessionData();
  setLoginForm();
}
