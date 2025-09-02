#include "mainwindow.h"
#include "chooseLogRegScreen.h"
#include "ui_mainwindow.h"

int MainWindow::kInstanceCount = 0;

MainWindow::MainWindow(std::shared_ptr<QtSession> sessionPtr, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  ui->setupUi(this);
  kInstanceCount++;

  if (sessionPtr)
    _sessionPtr = sessionPtr;

}

MainWindow::~MainWindow() {
  delete ui;
  kInstanceCount--;
  if (kInstanceCount <= 0)
    qApp->exit(0);
}

MainWindow *MainWindow::createSession(std::shared_ptr<QtSession> sessionPtr) {

  chooseLogRegScreen s(sessionPtr);
  auto result = s.exec();

  if (result == QDialog::Rejected)
    return nullptr;

  auto w = new MainWindow(sessionPtr);
  w->setAttribute(Qt::WA_DeleteOnClose);
  return w;
}
