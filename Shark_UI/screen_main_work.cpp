#include "screen_main_work.h"
#include "ui_screen_main_work.h"
#include "screen_chat_list.h"
#include "screen_chatting.h"

#include <QTimeZone>

#include "model_chat_list.h"
#include "model_chat_list_delegate.h"
#include "model_chat_messages.h"
#include "model_user_list.h"
#include "model_user_list_delegate.h"
#include "system/date_time_utils.h"

#include "message/message_content.h"
#include "message/message_content_struct.h"

ScreenMainWork::ScreenMainWork(QWidget *parent)
    : QWidget(parent), ui(new Ui::ScreenMainWork) {

  ui->setupUi(this);
  ui->leftPane->setMaximumWidth(QWIDGETSIZE_MAX);

  // стартовые доли (пример под 1200px ширину: 360/840)
  ui->splitter->setSizes({360, 1040});
  ui->splitter->setStyleSheet("QSplitter::handle{background:palette(mid);} "
                              "QSplitter::handle:horizontal{width:8px;}");
}

ScreenMainWork::~ScreenMainWork() { delete ui; }

void ScreenMainWork::setDatabase(std::shared_ptr<ClientSession> sessionPtr) {
  _sessionPtr = sessionPtr;
  connect(_sessionPtr.get(), &ClientSession::serverStatusChanged, this,
          &ScreenMainWork::onConnectionStatusChanged, Qt::QueuedConnection);
}

void ScreenMainWork::fillChatListModelWithData() {
  const auto listOfChat = _sessionPtr->getChatListQt();

  if (listOfChat.has_value()) {

    for (const auto &chat : listOfChat.value()) {

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
      for (const auto &participant : chat.second.participants) {

        // достаем логин и имя пользователя
        const auto &login = participant.login;

        // пропускаем активного пользователя
        if (login == _sessionPtr->getActiveUserCl()->getLogin())
          continue;

        const auto &user = _sessionPtr->getInstance().findUserByLogin(login);
        const auto &userName = user->getUserName();

        if (!first)
          participantsChatList += ", ";
        participantsChatList += QString::fromStdString(userName) + " (" +
                                QString::fromStdString(login) + ")";
        first = false;
      } // for

      // добавляем количество непрочитанных
      const auto &unreadMessCount =
          _sessionPtr->getInstance()
              .getChatById(chat.second.chatId)
              ->getUnreadMessageCount(_sessionPtr->getActiveUserCl());
      unreadCount = (unreadMessCount);

      infoText += ", новых = " + QString::number(unreadCount);

      // добавляем время последнего сообщения
      const auto dateTimeStamp = formatTimeStampToString(chat.first, true);
      infoText += ", последнее: " + QString::fromStdString(dateTimeStamp);

      lastTime = static_cast<std::int64_t>(chat.first);

      isMuted = false;

      _ChatListModel->fillChatListItem(participantsChatList, infoText,
                                       unreadCount, isMuted, lastTime,
                                       chat.second.chatId);

    } // for listOfChat

  } // if has_value

  //   доделать вывод сообщения что нет чатов
  else
    return;
}

void ScreenMainWork::fillUserListModelWithData() {

  const auto users = _sessionPtr->getInstance().getUsers();

  if (users.size()) {

    for (const auto &user_ptr : users) {

      if (user_ptr != nullptr) {
        const QString login = QString::fromStdString(user_ptr->getLogin());
        const QString name = QString::fromStdString(user_ptr->getUserName());
        const QString email = QString::fromStdString(user_ptr->getEmail());
        const QString phone = QString::fromStdString(user_ptr->getPhone());
        const QString disableReason =
            QString::fromStdString(user_ptr->getUserName());
        bool isActive = user_ptr->getIsActive();
        const std::int64_t disableAt = user_ptr->getDisabledAt();
        const std::int64_t bunUntil = user_ptr->getBunUntil();

        _userListModel->fillUserItem(login, name, email, phone, disableReason,
                                     isActive, disableAt, bunUntil);
      } // if user_ptr
    } // for users

  } // if users
}

void ScreenMainWork::fillMessageModelWithData() {
  // взяли index выбранной строки в списке чатов
  QModelIndex index = ui->chatListTab->currentIndex();

  // если список пуст
  if (!index.isValid())
    return;

  // достали chatId
  QVariant value = index.data(ChatListModel::ChatIdRole);
  std::size_t chatId = static_cast<size_t>(value.toLongLong());

  const auto messages =
      _sessionPtr->getInstance().getChatById(chatId)->getMessages();

  for (const auto &message : messages) {

    if (message.second != nullptr) {

      QString messageText = "";

      const auto &content = message.second->getContent();
      if (!content.empty()) {
        const auto &textContent =
            std::dynamic_pointer_cast<MessageContent<TextContent>>(content[0]);

        if (textContent)
          messageText =
              QString::fromStdString(textContent->getMessageContent()._text);
      }
      const auto sender = message.second->getSender();
      const auto sender_ptr = sender.lock();
      QString senderLogin = "";
      QString senderName = "";
      if (!sender_ptr) {
        senderLogin = "Удален";
        senderName = "";
      } else {
        senderLogin = QString::fromStdString(sender_ptr->getLogin());
        senderName = QString::fromStdString(sender_ptr->getUserName());
      }
      std::int64_t timeStamp = static_cast<int64_t>(message.first);
      std::size_t messageId =
          static_cast<size_t>(message.second->getMessageId());

      _MessageModel->fillMessageItem(messageText, senderLogin, senderName,
                                     timeStamp, messageId);

    } // if message
  } // for messages
}

void ScreenMainWork::onConnectionStatusChanged(bool connectionStatus,
                                               ServerConnectionMode mode) {
  if (connectionStatus) {
    ui->serverStatusLabelRound->setStyleSheet(
        "background-color: green; border-radius: 8px;");
    ui->serverStatusLabel->setText("server online");
  } else {
    ui->serverStatusLabelRound->setStyleSheet(
        "background-color: red; border-radius: 8px;");
    ui->serverStatusLabel->setText("server offline");
  }
}

void ScreenMainWork::createSession() {
  ui->chatUserTabWidget->setTabEnabled(0, true);
  ui->chatUserTabWidget->setCurrentIndex(0);

  ui->chatUserDataStackedWidget->setCurrentIndex(0);

  connect(_sessionPtr.get(), &ClientSession::serverStatusChanged, this,
          &ScreenMainWork::onConnectionStatusChanged, Qt::QueuedConnection);

  ui->serverStatusLabelRound->setStyleSheet(
      "background-color: green; border-radius: 8px;");
  ui->serverStatusLabel->setText("server online");

  ui->loginLabel->setText(
      " Логин: " +
      QString::fromStdString(_sessionPtr->getActiveUserCl()->getLogin()));
  ui->nameLabel->setText(
      "Имя: " +
      QString::fromStdString(_sessionPtr->getActiveUserCl()->getUserName()));

  // окно работы со списком чатов
  _ChatListModel = new ChatListModel(ui->chatListTab);

  fillChatListModelWithData();
  ui->chatListTab->setModel(_ChatListModel);

  connect(ui->chatListTab, &ScreenChatList::currentChatIndexChanged,
          ui->editChatPage, &ScreenChatting::onChatCurrentChanged);

  // окно работы с адресной книгой
  _userListModel = new UserListModel(ui->userListView);
  fillUserListModelWithData();

  ui->userListView->setModel(_userListModel);

  // Назначает делегат отрисовки элементов списка.
  ui->userListView->setItemDelegate(new UserListItemDelegate(ui->userListView));

  // Отключает “одинаковую высоту для всех строк
  ui->userListView->setUniformItemSizes(false); // высоту задаёт делегат

  // Убирает стандартный промежуток между строками
  ui->userListView->setSpacing(0); // разделитель рисуем сами

  // Заставляет прокрутку работать по пикселям, а не по строкам.
  ui->userListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

  // Даёт виджету события движения мыши даже без нажатия кнопок.
  ui->userListView->setMouseTracking(true);

  ui->userListView->setStyleSheet("QListView { background-color: #FFFFF0; }");

  ui->addressBookLabel->setText("Контакты из записной книжки");
  ui->findLineEdit->setPlaceholderText("Under Construction");
  ui->globalAddressBookCheckBox->setChecked(false);
}

void ScreenMainWork::on_chatUserTabWidget_currentChanged(int index) {
  if (index == 1) {
    ui->globalAddressBookCheckBox->setEnabled(true);

    ui->findPushButton->setEnabled(true);

    QPalette paletteLineEdit = ui->findLineEdit->palette();
    paletteLineEdit.setColor(QPalette::PlaceholderText, QColor("#000000"));
    ui->findLineEdit->setPalette(paletteLineEdit);

    ui->findLineEdit->setEnabled(true);
    ui->findLineEdit->setClearButtonEnabled(true);
    ui->findLineEdit->clear();
    ui->findLineEdit->setPlaceholderText("Поиск...");
  } else {
    ui->globalAddressBookCheckBox->setEnabled(false);

    ui->findPushButton->setEnabled(false);

    QPalette paletteLineEdit = ui->findLineEdit->palette();
    paletteLineEdit.setColor(QPalette::PlaceholderText, QColor("#9AA0A6"));
    ui->findLineEdit->setPalette(paletteLineEdit);
    ui->findLineEdit->setEnabled(false);
    ui->findLineEdit->setPlaceholderText("under construction");
  }
}
