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
          &MainWindow::onRejectedRequested);
  connect(ui->pageLogin, &ScreenLogin::accepted, this, &MainWindow::onLoggedIn);

  connect(ui->pageRegister, &ScreenRegister::loginRequested, this,
          &MainWindow::setLoginForm);
  connect(ui->pageRegister, &ScreenRegister::rejected, this,
          &MainWindow::onRejectedRequested);

  setLoginForm();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setLoginForm() {
  ui->pageLogin->clearFields();
  QWidget *page =
      ui->mainWindowstackedWidget->findChild<QWidget *>("pageLogin");

#ifdef Q_OS_MAC
  // скрыть меню: очистить действия у нативного меню
  menuBar()->clear();
#else
  menuBar()->setVisible(false);
#endif

  ui->mainWindowstackedWidget->setCurrentWidget(page);
}

void MainWindow::setRegistrationForm() {
  ui->pageRegister->clearFields();
  QWidget *page = ui->mainWindowstackedWidget->findChild<QWidget *>("pageRegister");

#ifdef Q_OS_MAC
  // скрыть меню: очистить действия у нативного меню
  menuBar()->clear();
#else
  menuBar()->setVisible(false);
#endif

  ui->mainWindowstackedWidget->setCurrentWidget(page);
}

void MainWindow::setworkForm() {
  QWidget *page = ui->mainWindowstackedWidget->findChild<QWidget *>("pageWork");

#ifdef Q_OS_MAC
  // macOS: возвращаем меню — повторно подвешиваем QMenu из .ui
  QMenuBar *mb = menuBar();
  if (mb->actions().isEmpty()) {
    mb->addAction(ui->menu->menuAction());
  }
#else
  menuBar()->setVisible(true);
#endif

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
  ui->pageWork->createSession();
  setworkForm();
}

void MainWindow::on_exitAction_triggered() {
  _sessionPtr->resetSessionData();
  setLoginForm();
}
