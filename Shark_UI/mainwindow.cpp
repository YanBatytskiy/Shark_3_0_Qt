#include "mainwindow.h"
#include "chooseLogRegScreen.h"
#include "ui_mainwindow.h"
#include "model_chat_list.h"
#include "system/date_time_utils.h"
#include <QTimeZone>
#include "model_chat_list_itemdelegate.h"

int MainWindow::kInstanceCount = 0;

MainWindow::MainWindow(std::shared_ptr<ClientSession> sessionPtr, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)  {

  ui->setupUi(this);
  kInstanceCount++;

  if (!sessionPtr) { qWarning() << "ClientSession is null"; return; }
  _sessionPtr = std::move(sessionPtr);
  connect(_sessionPtr.get(), &ClientSession::serverStatusChanged,
          this, &MainWindow::onConnectionStatusChanged,
          Qt::QueuedConnection);

    ui->serverStatusLabelRound->setStyleSheet("background-color: green; border-radius: 8px;");
    ui->serverStatusLabel->setText("server online");

  ui->loginLabel->setText(" Логин: " + QString::fromStdString(_sessionPtr->getActiveUserCl()->getLogin()));
  ui->nameLabel->setText("Имя: " + QString::fromStdString(_sessionPtr->getActiveUserCl()->getUserName()));

  _ChatListModel = new ChatListModel(ui->chatListView);
  fillChatListModelWithData();

  ui->chatListView->setModel(_ChatListModel);
  ui->chatListView->setItemDelegate(new ChatListItemDelegate(ui->chatListView));
  ui->chatListView->setUniformItemSizes(false);
  ui->chatListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
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

void MainWindow::fillChatListModelWithData()
{
    const auto listOfChat = _sessionPtr->getChatListQt();

  if (listOfChat.has_value()) {


    for (const auto& chat : listOfChat.value()) {

      QString participantsChatList;
      QString infoText;
      int unreadCount;
      bool isMuted;
      std::int64_t lastTime;

      // добавляем номер чата
      infoText = "Chat_Id = " + QString::number(chat.second.chatId);

             // собираем строку участников с логинами и именами
      participantsChatList = "Имя (Логин: ";
      bool first = true;
      for (const auto& participant : chat.second.participants) {

        //достаем логин и имя пользователя
        const auto& login = participant.login;
        const auto &user = _sessionPtr->getInstance().findUserByLogin(login);
        const auto& userName = user->getUserName();

        if (!first) participantsChatList += ", ";
        participantsChatList += QString::fromStdString(userName) + " (" + QString::fromStdString(login) + ")";
        first = false;
      } // for

      //добавляем количество непрочитанных
      const auto& unreadMessCount =_sessionPtr->getInstance().getChatById(chat.second.chatId)->getUnreadMessageCount(_sessionPtr->getActiveUserCl());
      unreadCount = (unreadMessCount);

      infoText += ", новых = " + QString::number(unreadCount);

      //добавляем время последнего сообщения
      const auto dateTimeStamp = formatTimeStampToString(chat.first, true);
      infoText += ", последнее: " + QString::fromStdString(dateTimeStamp);

      lastTime = static_cast<std::int64_t>(chat.first);

      isMuted = false;

      _ChatListModel->fillChatListItem(participantsChatList, infoText, unreadCount, isMuted, lastTime);

    }// for listOfChat

  } // if has_value

  //   доделать вывод сообщения что нет чатов
  else return;
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

