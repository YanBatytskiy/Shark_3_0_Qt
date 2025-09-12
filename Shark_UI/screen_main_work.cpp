#include "screen_main_work.h"
#include "ui_screen_main_work.h"
#include "screen_chat_list.h"
#include "screen_chatting.h"

#include <QTimeZone>
#include <QMessageBox>
#include <QItemSelectionModel>

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
  ui->splitter->setStyleSheet("QSplitter::handle{background:palette(mid);} "
                              "QSplitter::handle:horizontal{width:8px;}");
}

ScreenMainWork::~ScreenMainWork() { delete ui; }

void ScreenMainWork::setDatabase(std::shared_ptr<ClientSession> sessionPtr) {

  _sessionPtr = sessionPtr;
  connect(_sessionPtr.get(), &ClientSession::serverStatusChanged, this,
          &ScreenMainWork::onConnectionStatusChanged, Qt::QueuedConnection);
}

void ScreenMainWork::fillChatListModelWithData(bool allChats) {

  _ChatListModel->clear();

  const auto listOfChat = _sessionPtr->getChatListQt();

  if (listOfChat.has_value()) {

    const QModelIndex& index = ui->userListView->currentIndex();

    std::string contactLogin;

    if (index.isValid()) {
      contactLogin = index.data(UserListModel::LoginRole).toString().toStdString();
    } else contactLogin.clear();

    for (const auto &chat : listOfChat.value()) {

      QString participantsChatList;
      QString infoText;

      int unreadCount;
      bool isMuted;
      std::int64_t lastTime;

      // добавляем номер чата
      const auto chatIdStr = QString::number(chat.second.chatId);

      // добавляем количество непрочитанных
      unreadCount =
          _sessionPtr->getInstance()
              .getChatById(chat.second.chatId)
              ->getUnreadMessageCount(_sessionPtr->getActiveUserCl());


      // добавляем время последнего сообщения
      lastTime = static_cast<std::int64_t>(chat.first);
      const auto dateTimeStamp = formatTimeStampToString(chat.first, true);

      infoText = _ChatListModel->buildInfoTextForRow(chatIdStr, QString::number(unreadCount), QString::fromStdString(dateTimeStamp));


      // собираем строку участников с логинами и именами
      participantsChatList = "Имя (Логин: ";
      bool first = true;
      bool isContactLogin = false;
      for (const auto &participant : chat.second.participants) {

        // достаем логин и имя пользователя
        const auto &login = participant.login;

        // пропускаем активного пользователя
        if (login == _sessionPtr->getActiveUserCl()->getLogin())
          continue;

        // определяем наличие выбранного контакта в списке участников чата
        if (!allChats && login == contactLogin) isContactLogin = true;


        const auto &user = _sessionPtr->getInstance().findUserByLogin(login);
        const auto &userName = user->getUserName();

        if (!first)
          participantsChatList += ", ";

        participantsChatList += QString::fromStdString(userName) + " (" +
                                QString::fromStdString(login) + ")";
        first = false;
      } // for


      if (!allChats && !isContactLogin) continue;

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

      if (user_ptr != nullptr)
        if (user_ptr->getLogin() != _sessionPtr->getActiveUserCl()->getLogin())
        {
          const QString login = QString::fromStdString(user_ptr->getLogin());
          const QString name = QString::fromStdString(user_ptr->getUserName());
          const QString email = QString::fromStdString(user_ptr->getEmail());
          const QString phone = QString::fromStdString(user_ptr->getPhone());
          const QString disableReason = QString::fromStdString(user_ptr->getUserName());
          bool isActive = user_ptr->getIsActive();
          const std::int64_t disableAt = user_ptr->getDisabledAt();
          const std::int64_t bunUntil = user_ptr->getBunUntil();

          _userListModel->fillUserItem(login, name, email, phone, disableReason, isActive, disableAt, bunUntil);
        } // if user_ptr
    } // for users

  } // if users
}

void ScreenMainWork::fillMessageModelWithData(std::size_t chatId) {

  _MessageModel->clear();

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

  fillChatListModelWithData(true);
  ui->chatListTab->setModel(_ChatListModel);

// окно работы с адресной книгой
  setupUserList();

         //работа с сообщениями чата
  setupScreenChatting();


  ui->chatListTab->setFocus();
}

void ScreenMainWork::setupUserList()
{
// окно работы с адресной книгой
  _userListModel = new UserListModel(ui->userListView);

  fillUserListModelWithData();

  ui->userListView->setModel(_userListModel);

  //подкючили модель ко второму виджету для отображения списка чатов конкретного контакта
  ui->userViewPage->findChild<ScreenChatList*>("chatListForUserDataWidget")->setModel(_ChatListModel);

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

  connect(this, &ScreenMainWork::currentUserIndexChanged, ui->userViewPage, &ScreenUserData::setUserDataToLabels);

  //устанавливаем сигналы изменения индекса чата

  auto selectMode = ui->userListView->selectionModel();

  QObject::connect(selectMode, &QItemSelectionModel::currentChanged,
                   ui->userViewPage, &ScreenUserData::setUserDataToLabels);


  auto chatListInUser =
      ui->userViewPage->findChild<ScreenChatList*>("chatListForUserDataWidget");


  QObject::connect(selectMode, &QItemSelectionModel::currentChanged,
                   chatListInUser, &ScreenChatList::onUserListIndexChanged);

  QObject::connect(chatListInUser, &ScreenChatList::UserListIdChanged,
                   this, [this](auto){ fillChatListModelWithData(false); });

}




void ScreenMainWork::setupScreenChatting()
{
  //работа с сообщениями чата

//создаем модель сообщений
  _MessageModel = new MessageModel(ui->editChatPage);


//работаем с окном со всеми чатами пользователя

 // привязали модель к виду на странице со всеми чатами
  ui->editChatPage->setModel(_MessageModel);

// заполнили модель данными конкретного чата
  const auto &index = ui->chatListTab->currentIndex();

  if (index.isValid()) {
    const auto& currentChatId =
        static_cast<size_t>(index.data(ChatListModel::ChatIdRole).toLongLong());

    fillMessageModelWithData(currentChatId);
  }

  //связываем сигнал смены чата в списке чатов с методом вывода сообщений конкретного чата в форму
  connect(ui->chatListTab, &ScreenChatList::currentChatIndexChanged,
          ui->editChatPage, &ScreenChatting::onChatCurrentChanged,Qt::UniqueConnection);

  //сбрасываем количество непрочитанных у первого чата в списке
  resetCountUnreadMessagesCommmand();

  // связь из окна редактирования чата c методом заполнения модели сообщений
  connect(ui->editChatPage,&ScreenChatting::ChatListIdChanged,
          this,&ScreenMainWork::fillMessageModelWithData,
          Qt::UniqueConnection);

//связь с сигналом очистки непрочитанных
  connect(ui->chatListTab, &ScreenChatList::currentChatIndexChanged,
          this, &ScreenMainWork::resetCountUnreadMessagesCommmand, Qt::UniqueConnection);

  //связываем сигнал от экрана редактирования сообщения с методом отправки сообщения
  connect(ui->editChatPage, &ScreenChatting::sendMessageSignal,
          this, &ScreenMainWork::sendMessageCommmand,Qt::UniqueConnection);



//работаем с окном с чатами конкретного контакта



//устанавливаем сигналы изменения индекса чата

// экран «чаты контакта»
  const auto chatListInUser = ui->userViewPage->findChild<ScreenChatList *>("chatListForUserDataWidget");
  if (!chatListInUser) return;

// указатель на объект содержащий окно сообщний в режиме чата отдельного контакта адресной книги
  auto MessageChattingInUser = ui->userViewPage->findChild<ScreenChatting *>("screenChattingInUserDataWidget");
   if (!MessageChattingInUser) return;

          // привязали модель ко второму виджету для отображения списка чатов конкретного контакта
   MessageChattingInUser->setModel(_MessageModel);

  auto selectMode = chatListInUser->getSelectionModel();
   if (!selectMode)
    return;

          // связь: список чатов контакта -> окно сообщений контакта
   connect(chatListInUser, &ScreenChatList::currentChatIndexChanged, MessageChattingInUser,
           &ScreenChatting::onChatCurrentChanged);

          // связь: окно сообщений контакта -> подгрузка сообщений выбранного чата
   connect(
       MessageChattingInUser, &ScreenChatting::ChatListIdChanged, this,
       [this](std::size_t chatId) { fillMessageModelWithData(chatId); });

   connect(chatListInUser, &ScreenChatList::UserListIdChanged,
           this, [this](auto){ fillChatListModelWithData(false); });

          // связь с сигналом очистки непрочитанных
   connect(chatListInUser, &ScreenChatList::currentChatIndexChanged, this,
           &ScreenMainWork::resetCountUnreadMessagesCommmand, Qt::UniqueConnection);

          // связываем сигнал от экрана редактирования сообщения с методом отправки сообщения
   connect(MessageChattingInUser,
           &ScreenChatting::sendMessageSignal,
           this,
           &ScreenMainWork::sendMessageCommmand,
           Qt::UniqueConnection);
}
void ScreenMainWork::on_chatUserTabWidget_currentChanged(int index) {
  if (index == 1) {
    // userData
    ui->globalAddressBookCheckBox->setEnabled(true);
    ui->findPushButton->setEnabled(true);

    QPalette paletteLineEdit = ui->findLineEdit->palette();
    paletteLineEdit.setColor(QPalette::PlaceholderText, QColor("#000000"));
    ui->findLineEdit->setPalette(paletteLineEdit);

    ui->findLineEdit->setEnabled(true);
    ui->findLineEdit->setClearButtonEnabled(true);
    ui->findLineEdit->clear();
    ui->findLineEdit->setPlaceholderText("Поиск...");
    ui->chatUserDataStackedWidget->setCurrentIndex(1);

    const auto& userIdx = ui->userListView->currentIndex();

    if (userIdx.isValid()) {
      emit currentUserIndexChanged(userIdx);
    }
  } else {
    // chatList
    ui->globalAddressBookCheckBox->setEnabled(false);
    ui->findPushButton->setEnabled(false);

    QPalette paletteLineEdit = ui->findLineEdit->palette();
    paletteLineEdit.setColor(QPalette::PlaceholderText, QColor("#9AA0A6"));
    ui->findLineEdit->setPalette(paletteLineEdit);
    ui->findLineEdit->setEnabled(false);
    ui->findLineEdit->setPlaceholderText("under construction");
    ui->chatUserDataStackedWidget->setCurrentIndex(0);

    fillChatListModelWithData(true);

    auto *lv = ui->chatListTab->findChild<QListView*>("chatListView");

    if (lv) {
      if (auto *m = lv->model(); m && m->rowCount() > 0) {
        const QModelIndex first = m->index(0, 0);
        lv->setCurrentIndex(first);
      }
    }
  }
}

void ScreenMainWork::sendMessageCommmand()
{
  // выбираем актуальный индекс чата по источнику сигнала
  QModelIndex idx;
  if (auto s = qobject_cast<ScreenChatting*>(sender());
      s && s != ui->editChatPage) {
    auto cl = ui->userViewPage->findChild<ScreenChatList*>("chatListForUserDataWidget",
                                                            Qt::FindChildrenRecursively);
    idx = cl ? cl->currentIndex() : QModelIndex();
  } else {
    idx = ui->chatListTab->currentIndex();
  }

  if (!idx.isValid()) return;

  const auto& currentChatId =
      static_cast<size_t>(idx.data(ChatListModel::ChatIdRole).toLongLong());

  auto chat_ptr = _sessionPtr->getInstance().getChatById(currentChatId);

  std::vector<std::shared_ptr<User>> recipients;
  for (const auto &participant : chat_ptr->getParticipants()) {
    auto user_ptr = participant._user.lock();
    if (user_ptr && user_ptr != _sessionPtr->getActiveUserCl())
      recipients.push_back(user_ptr);
  }

         // берём текст из того редактора, где нажали кнопку
  QString newMessageText;
  if (auto s = qobject_cast<ScreenChatting*>(sender()); s) {
    if (auto ed = s->newMessageTextEditBlock())
      newMessageText = ed->toPlainText();
  } else {
    newMessageText = ui->editChatPage->newMessageTextEditBlock()->toPlainText();
  }
  if (newMessageText.trimmed().isEmpty()) return;

  std::vector<std::shared_ptr<IMessageContent>> iMessageContent;
  TextContent textContent = newMessageText.toStdString();
  MessageContent<TextContent> messageContentText(textContent);
  iMessageContent.push_back(std::make_shared<MessageContent<TextContent>>(messageContentText));

  auto newMessageTimeStamp = getCurrentDateTimeInt();
  Message newMessage(iMessageContent, _sessionPtr->getActiveUserCl(), newMessageTimeStamp, 0);

  const auto& newMessageId =
      _sessionPtr->createMessageCl(newMessage, chat_ptr, _sessionPtr->getInstance().getActiveUser());

  if (newMessageId == 0) {
    QMessageBox::warning(this, tr("Ошибка!"), tr("Невозможно отправить сообщение."));
    return;
  }

  _MessageModel->fillMessageItem(
      newMessageText,
      QString::fromStdString(_sessionPtr->getActiveUserCl()->getLogin()),
      QString::fromStdString(_sessionPtr->getActiveUserCl()->getUserName()),
      newMessageTimeStamp,
      newMessageId
      );

         // чистим редактор-источник
  if (auto s = qobject_cast<ScreenChatting*>(sender()); s) {
    if (auto ed = s->newMessageTextEditBlock()) ed->setText("");
  } else {
    ui->editChatPage->newMessageTextEditBlock()->setText("");
  }

         // заменили в модели время последнего сообщения
  _ChatListModel->setLastTime(idx.row(), newMessageTimeStamp);
}

void ScreenMainWork::resetCountUnreadMessagesCommmand()
{
// так как пользователь вошел в чат и все прочитал, сбрасываем количество новых сообщений


         //взяли индекс строки
const auto& idx = ui->chatListTab->currentIndex();

  //достали chatId
  if (!idx.isValid()) return;

  const auto& currentChatId =
      static_cast<size_t>(idx.data(ChatListModel::ChatIdRole).toLongLong());

//доастали указатель на чат
  const auto& chat_ptr = _sessionPtr->getInstance().getChatById(currentChatId);

  //проверили, все ли сообщения прочитаны
  const auto &lastMessageId = chat_ptr->getMessages().rbegin()->second->getMessageId();
  const auto &lastReadMessageId = chat_ptr->getLastReadMessageId(_sessionPtr->getInstance().getActiveUser());

  if (lastMessageId != lastReadMessageId) {
    chat_ptr->setLastReadMessageId(_sessionPtr->getActiveUserCl(), lastMessageId);

    //подали команду серверу на установку последнего прочитанного сообщения
    _sessionPtr->sendLastReadMessageFromClient(chat_ptr, lastMessageId);

  // заменили в модели количество непрочатанных и обновили элемент списка
    _ChatListModel->setUnreadCount(idx.row(),0);
  }
}
