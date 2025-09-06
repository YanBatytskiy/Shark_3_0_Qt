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
  ui->chatUserTabWidget->setTabEnabled(0,true);
  ui->chatUserTabWidget->setCurrentIndex(0);

  qDebug() << "count" << ui->chatUserTabWidget->count()
           << "idx" << ui->chatUserTabWidget->currentIndex()
           << "tab0Enabled" << ui->chatUserTabWidget->isTabEnabled(0);

  QObject::connect(ui->chatUserTabWidget, &QTabWidget::currentChanged,
                   this, [](int i){ qDebug() << "currentChanged ->" << i; });

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

//окно работы со списком чатов
  _ChatListModel = new ChatListModel(ui->chatListView);
  fillChatListModelWithData();

  ui->chatListView->setModel(_ChatListModel);

//Назначает делегат отрисовки элементов списка.
  ui->chatListView->setItemDelegate(new ChatListItemDelegate(ui->chatListView));

//Отключает “одинаковую высоту для всех строк
  ui->chatListView->setUniformItemSizes(false);               // высоту задаёт делегат

//Убирает стандартный промежуток между строками
  ui->chatListView->setSpacing(0);                            // разделитель рисуем сами

  //Заставляет прокрутку работать по пикселям, а не по строкам.
  ui->chatListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

         // Даёт виджету события движения мыши даже без нажатия кнопок.
  ui->chatListView->setMouseTracking(true);

  ui->chatListView->setStyleSheet("QListView { background-color: #FFFFF0; }");

  // окно работы с адресной книгой
  ui->addressBookLabel->setText("Контакты из записной книжки");
  ui->findLineEdit->setPlaceholderText("Under Construction");
  ui->globalAddressBookCheckBox->setChecked(false);


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
      QString infoText;  void on_globalAddressBookCheckBox_checkStateChanged(const Qt::CheckState &arg1);

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

        //пропускаем активного пользователя
        if (login == _sessionPtr->getActiveUserCl()->getLogin()) continue;

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

void MainWindow::on_chatUserTabWidget_currentChanged(int index)
{
  if (index == 1) {
    ui->findLineEdit->setEnabled(true);
    QPalette paletteLineEdit = ui->findLineEdit->palette();
    paletteLineEdit.setColor(QPalette::PlaceholderText, QColor("9AA0A6"));
    ui->findLineEdit->setPalette(paletteLineEdit);
    ui->findLineEdit->setClearButtonEnabled(true);
    ui->findLineEdit->clear();
    ui->findLineEdit->setPlaceholderText("Поиск...");
    ui->findPushButton->setEnabled(true);
    ui->globalAddressBookCheckBox->setEnabled(true);
  }
  else {
    ui->findLineEdit->setEnabled(false);
    ui->findPushButton->setEnabled(false);
    ui->globalAddressBookCheckBox->setEnabled(false);
  }
}

