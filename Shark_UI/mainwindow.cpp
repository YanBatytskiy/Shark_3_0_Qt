#include "mainwindow.h"
#include "chooseLogRegScreen.h"
#include "ui_mainwindow.h"
#include "model_chat.h"


int MainWindow::kInstanceCount = 0;

MainWindow::MainWindow(std::shared_ptr<ClientSession> sessionPtr, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)  {

  ui->setupUi(this);
  kInstanceCount++;

  if (sessionPtr)
    _sessionPtr = sessionPtr;

  connect(_sessionPtr.get(), &ClientSession::serverStatusChanged, this, &MainWindow::onConnectionStatusChanged, Qt::QueuedConnection);

    ui->serverStatusLabelRound->setStyleSheet("background-color: green; border-radius: 8px;");
    ui->serverStatusLabel->setText("server online");

  ui->loginLabel->setText(" Логин: " + QString::fromStdString(_sessionPtr->getActiveUserCl()->getLogin()));
  ui->nameLabel->setText("Имя: " + QString::fromStdString(_sessionPtr->getActiveUserCl()->getUserName()));

  _chatModel = new ChatModel(ui->chatListView);
  ui->chatListView->setModel(_chatModel);

}

MainWindow::~MainWindow() {
  delete ui;
  kInstanceCount--;
  if (kInstanceCount <= 0)
    qApp->exit(0);
}

MainWindow *MainWindow::createSession(std::shared_ptr<ClientSession> sessionPtr) {

  chooseLogRegScreen s(sessionPtr);
  auto result = s.exec();

  if (result == QDialog::Rejected)
    return nullptr;

  auto w = new MainWindow(sessionPtr);
  w->setAttribute(Qt::WA_DeleteOnClose);
  return w;
}

void MainWindow::onConnectionStatusChanged(bool connectionStatus, ServerConnectionMode mode)
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

void MainWindow::on_exitAction_triggered()
{
  this->close();

}

