#include "screen_main_work.h"
#include "ui_screen_main_work.h"
#include "screen_chat_list.h"
#include "screen_chatting.h"

#include <QTimeZone>
#include <QMessageBox>
#include <QItemSelectionModel>

#include "model_chat_list.h"
#include "model_chat_messages.h"
#include "model_user_list.h"
#include "model_user_list_delegate.h"
#include "system/date_time_utils.h"

#include "message/message_content.h"
#include "message/message_content_struct.h"

#include "dto/dto_struct.h"

#include "screen_new_chat_participants.h"

ScreenMainWork::ScreenMainWork(QWidget *parent)
    : QWidget(parent), ui(new Ui::ScreenMainWork) {

  ui->setupUi(this);

  if (auto w = ui->mainWorkPageUserDataView
                   ->findChild<ScreenNewChatParticipants *>("ScreenNewChatParticipantsWidget")) {

    // связь: кнопка начать создать новый чат в списке контактов - слот на включение формы и заполнение её
    connect(this, &ScreenMainWork::signalStartNewChat,
            w, &ScreenNewChatParticipants::slotCollectParticipantsForNewChat);

    // связь: кнопка отменить создание нового чата - слот на выключение формы и включение списка чатов и окна редактирования сообщений
    connect(w, &ScreenNewChatParticipants::signalCancelNewChat, this, &ScreenMainWork::slotCancelNewChat);

    // связь: сигнал добавления контакта - слот добавления контакта в список
    connect(this, &ScreenMainWork::signalAddContactToNewChat, w, &ScreenNewChatParticipants::slotAddContactToParticipantsList);
  }
}

ScreenMainWork::~ScreenMainWork() { delete ui; }

void ScreenMainWork::setDatabase(std::shared_ptr<ClientSession> sessionPtr) {

  _sessionPtr = sessionPtr;
  ui->findLineEdit->setEnabled(false);
  ui->mainWorkChatUserTabWidget->setCurrentIndex(0);
  ui->mainWorkRightStackedWidget->setCurrentIndex(0);

  connect(_sessionPtr.get(), &ClientSession::serverStatusChanged, this,
          &ScreenMainWork::onConnectionStatusChanged, Qt::QueuedConnection);
}

void ScreenMainWork::fillChatListModelWithData(bool allChats) {

  const auto listOfChat = _sessionPtr->getChatListQt();

  if (listOfChat.has_value()) {

    std::string contactLogin;

    if (!allChats) {
      const QModelIndex &idx = ui->mainWorkUsersList->currentIndex();

      if (idx.isValid()) {
        contactLogin = idx.data(UserListModel::LoginRole).toString().toStdString();
      } else
        contactLogin.clear();
    }

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

  if (auto sm = ui->mainWorkUsersList->selectionModel()) {
    QSignalBlocker b(sm);
    _userListModel->clear();
  }

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

void ScreenMainWork::slotCancelNewChat() {
  // экран «чаты контакта»
  const auto chatListInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatList *>(
      "ScreenUserDataChatsListWidget");
  if (!chatListInUser)
    return;

  // указатель на объект содержащий окно сообщний в режиме чата отдельного контакта адресной книги
  auto MessageChattingInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatting *>(
      "ScreenUserDataMessagesListWidget");
  if (!MessageChattingInUser)
    return;

  chatListInUser->setEnabled(true);
  chatListInUser->setVisible(true);

  MessageChattingInUser->setEnabled(true);
  MessageChattingInUser->setVisible(true);

  ui->createNewChatPushButton->setEnabled(true);

  auto selectModel = ui->mainWorkUsersList->selectionModel();
  if (selectModel) {
    selectModel->setCurrentIndex(selectModel->model()->index(0, 0),
                                 QItemSelectionModel::ClearAndSelect);
  };

  auto ChatListInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatList *>(
      "ScreenUserDataChatsListWidget");

  if (!ChatListInUser)
    return;

  selectModel = ChatListInUser->getSelectionModel();

  if (selectModel) {
    selectModel->setCurrentIndex(selectModel->model()->index(0, 0),
                                 QItemSelectionModel::ClearAndSelect);
  };
  ui->mainWorkChatUserTabWidget->setTabEnabled(0, true);
}

void ScreenMainWork::slotFindContactsByPart() {

  if (!ui->findLineEdit->isEnabled())
    return;

  auto textToFind = ui->findLineEdit->text().toStdString();

  if (textToFind == "")
    return;

  std::vector<UserDTO> userListDTO;
  userListDTO.clear();

  userListDTO = _sessionPtr->findUserByTextPartOnServerCl(textToFind);

  if (auto sm = ui->mainWorkUsersList->selectionModel()) {
    QSignalBlocker b(sm);
    _userListModel->clear();
  }

  if (userListDTO.empty())
    return;

  for (const auto &userDTO : userListDTO) {

    const QString login = QString::fromStdString(userDTO.login);
    const QString name = QString::fromStdString(userDTO.userName);
    const QString email = QString::fromStdString(userDTO.email);
    const QString phone = QString::fromStdString(userDTO.phone);
    const QString disableReason = QString::fromStdString(userDTO.disable_reason);
    bool isActive = userDTO.is_active;
    const std::int64_t disableAt = userDTO.disabled_at;
    const std::int64_t bunUntil = userDTO.ban_until;

    _userListModel->fillUserItem(login, name, email, phone, disableReason, isActive, disableAt, bunUntil);

  } // for userLisstDTO
}

void ScreenMainWork::createSession() {

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
  _ChatListModel = new ChatListModel(this);
  ui->mainWorkTabChatsList->setModel(_ChatListModel);

         // подкючили модель ко второму виджету для отображения списка чатов конкретного контакта
  ui->mainWorkPageUserDataView->findChild<ScreenChatList*>("ScreenUserDataChatsListWidget")->setModel(_ChatListModel);
\
      fillChatListModelWithData(true);

// окно работы с адресной книгой
  setupUserList();

         //работа с сообщениями чата
  setupScreenChatting();


  ui->mainWorkTabChatsList->setFocus();
}

void ScreenMainWork::setupUserList()
{
// окно работы с адресной книгой
  _userListModel = new UserListModel(ui->mainWorkUsersList);
  ui->mainWorkUsersList->setModel(_userListModel);

  fillUserListModelWithData();


         // Назначает делегат отрисовки элементов списка.
  ui->mainWorkUsersList->setItemDelegate(new UserListItemDelegate(ui->mainWorkUsersList));

  //        // Отключает “одинаковую высоту для всех строк
  // ui->mainWorkUsersList->setUniformItemSizes(false); // высоту задаёт делегат

  // Убирает стандартный промежуток между строками
  ui->mainWorkUsersList->setSpacing(0); // разделитель рисуем сами

         // Заставляет прокрутку работать по пикселям, а не по строкам.
  ui->mainWorkUsersList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

         // Даёт виджету события движения мыши даже без нажатия кнопок.
  ui->mainWorkUsersList->setMouseTracking(true);

  ui->mainWorkUsersList->setStyleSheet("QListView { background-color: #FFFFF0; }");

  ui->addressBookLabel->setText("Контакты из записной книжки");
  ui->findLineEdit->setPlaceholderText("Under Construction");

  //достаем модель выбора второго списка чатов (списка чатов конкретного пользователя)
  auto selectMode = ui->mainWorkUsersList->selectionModel();

  // связь- изменение индекса при выборе контакта - заполнение окна данных контакта
  QObject::connect(selectMode, &QItemSelectionModel::currentChanged,
                   ui->mainWorkPageUserDataView, &ScreenUserData::setUserDataToLabels);

  // связь- изменение индекса при выборе контакта - заполнение окна списка чатов контакта
  QObject::connect(selectMode,
                   &QItemSelectionModel::currentChanged,
                   this, [this](auto) {
                     refillChatListModelWithData(false);

                     const auto chatListInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatList *>(
                         "ScreenUserDataChatsListWidget");

                     if (!chatListInUser)
                       return;

                     auto selectMode = chatListInUser->getSelectionModel();
                     if (!selectMode)
                       return;

                     selectMode->setCurrentIndex(selectMode->model()->index(0, 0),
                                                 QItemSelectionModel::ClearAndSelect);
                   });
}

void ScreenMainWork::setupScreenChatting() {
  //работа с сообщениями чата

//создаем модель сообщений
  _MessageModel = new MessageModel(ui->mainWorkPageChatting);


//работаем с окном со всеми чатами пользователя

 // привязали модель к виду на странице со всеми чатами
  ui->mainWorkPageChatting->setModel(_MessageModel);

// заполнили модель данными конкретного чата
  const auto &index = ui->mainWorkTabChatsList->getCcurrentChatIndex();

  if (index.isValid()) {
    const auto& currentChatId =
        static_cast<size_t>(index.data(ChatListModel::ChatIdRole).toLongLong());

    fillMessageModelWithData(currentChatId);
  }

  // связь: список чатов общий -> окно сообщений чата
  //!!!
  connect(
      ui->mainWorkTabChatsList, &ScreenChatList::signalCurrentChatIndexChanged, this,
      [this](auto) {
        const auto &idx = ui->mainWorkTabChatsList->getCcurrentChatIndex();
        if (idx.isValid()) {
          auto currentChatId = idx.data(ChatListModel::ChatIdRole).toLongLong();
          fillMessageModelWithData(currentChatId);
        }
      });

  //сбрасываем количество непрочитанных у первого чата в списке
  resetCountUnreadMessagesCommmand();

  //!!!
  // связь с сигналом очистки непрочитанных
  connect(ui->mainWorkTabChatsList,
          &ScreenChatList::signalCurrentChatIndexChanged,
          this,
          &ScreenMainWork::resetCountUnreadMessagesCommmand,
          Qt::UniqueConnection);

  //!!!
  // связь: сигнал отправить сообщение из общего списка чатов - метод отправки сообщения
  connect(ui->mainWorkPageChatting, &ScreenChatting::signalsendMessage, this, [this] {
    QModelIndex idx;

    idx = ui->mainWorkTabChatsList->getCcurrentChatIndex();
    if (!idx.isValid())
      return;

    const auto &currentChatId = static_cast<size_t>(
        idx.data(ChatListModel::ChatIdRole).toLongLong());

    QString newMessageText;
    newMessageText = ui->mainWorkPageChatting->getScreenChattingNewMessageTextEdit()->toPlainText();

    if (newMessageText.trimmed().isEmpty())
      return;

    sendMessageCommmand(idx, currentChatId, newMessageText);

    refillChatListModelWithData(true);

    const auto chatListInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatList *>(
        "ScreenUserDataChatsListWidget");

    if (!chatListInUser)
      return;

    auto selectMode = ui->mainWorkTabChatsList->getSelectionModel();
    if (!selectMode)
      return;

    selectMode->setCurrentIndex(selectMode->model()->index(0, 0),
                                QItemSelectionModel::ClearAndSelect);
    
    
    
  });

  // работаем с окном с чатами конкретного контакта

  // экран «чаты контакта»
  const auto chatListInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatList *>(
      "ScreenUserDataChatsListWidget");
  if (!chatListInUser)
    return;

  // указатель на объект содержащий окно сообщний в режиме чата отдельного контакта адресной книги
  auto MessageChattingInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatting *>(
      "ScreenUserDataMessagesListWidget");
  if (!MessageChattingInUser)
    return;

  // привязали модель ко второму виджету для отображения списка чатов конкретного контакта
  MessageChattingInUser->setModel(_MessageModel);

  // получаем SelectionModel от списка чатов конкретного пользователя
  auto selectMode = chatListInUser->getSelectionModel();
  if (!selectMode)
    return;

  // связь: список чатов контакта -> окно сообщений чата
  connect(chatListInUser,
          &ScreenChatList::signalCurrentChatIndexChanged,
          this,
          [this, chatListInUser](auto) {
            const auto &idx = chatListInUser->getCcurrentChatIndex();

            if (idx.isValid()) {
              auto currentChatId = idx.data(ChatListModel::ChatIdRole).toLongLong();
              fillMessageModelWithData(currentChatId);
            }
          });

  // связь с сигналом очистки непрочитанных
  connect(chatListInUser,
          &ScreenChatList::signalCurrentChatIndexChanged,
          this,
          &ScreenMainWork::resetCountUnreadMessagesCommmand,
          Qt::UniqueConnection);

  // связываем сигнал от экрана редактирования сообщения с методом отправки сообщения
  connect(MessageChattingInUser,
          &ScreenChatting::signalsendMessage,
          this,
          [this, chatListInUser, MessageChattingInUser] {
            const auto &idx = chatListInUser->getCcurrentChatIndex();

            if (!idx.isValid())
              return;

            const auto &currentChatId = static_cast<size_t>(
                idx.data(ChatListModel::ChatIdRole).toLongLong());

            QString newMessageText;
            newMessageText = MessageChattingInUser->getScreenChattingNewMessageTextEdit()
                                 ->toPlainText();

            if (newMessageText.trimmed().isEmpty())
              return;

            sendMessageCommmand(idx, currentChatId, newMessageText);

            if (!chatListInUser)
              return;

            auto selectMode = chatListInUser->getSelectionModel();
            if (!selectMode)
              return;

            selectMode->setCurrentIndex(selectMode->model()->index(0, 0),
                                        QItemSelectionModel::ClearAndSelect);
            
          });
}

void ScreenMainWork::refillChatListModelWithData(bool allChats) {
  if (auto sm = ui->mainWorkTabChatsList->getSelectionModel()) {
    QSignalBlocker b1(sm);
    auto clInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatList *>(
        "ScreenUserDataChatsListWidget");
    QSignalBlocker b2(clInUser ? clInUser->getSelectionModel() : nullptr);

    _ChatListModel->clear();
    fillChatListModelWithData(allChats);      
  }
}

void ScreenMainWork::on_mainWorkChatUserTabWidget_currentChanged(int index) {
  if (index == 1) {
    // userData

    ui->findPushButton->setEnabled(true);

    ui->addUserToChatPushButton->setVisible(false);

    QPalette paletteLineEdit = ui->findLineEdit->palette();
    paletteLineEdit.setColor(QPalette::PlaceholderText, QColor("#000000"));
    ui->findLineEdit->setPalette(paletteLineEdit);

    const auto userDataView = ui->mainWorkPageUserDataView->findChild<ScreenUserData *>(
        "ScreenUserDataUserDataWidget");

    ui->findLineEdit->setEnabled(true);

    ui->findLineEdit->setClearButtonEnabled(true);
    ui->findLineEdit->clear();
    ui->findLineEdit->setPlaceholderText("Поиск...");
    ui->mainWorkRightStackedWidget->setCurrentIndex(1);

    fillUserListModelWithData();

    const auto &userIdx = ui->mainWorkUsersList->currentIndex();

    const auto chatListInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatList *>(
        "ScreenUserDataChatsListWidget");

    if (!chatListInUser)
      return;

    auto selectMode = chatListInUser->getSelectionModel();
    if (!selectMode)
      return;

    selectMode->setCurrentIndex(selectMode->model()->index(0, 0),
                                QItemSelectionModel::ClearAndSelect);

    if (userIdx.isValid()) {
      ui->mainWorkUsersList->setCurrentIndex(ui->mainWorkUsersList->model()->index(0, 0));
    }
  } else {
    // chatList
    ui->findPushButton->setEnabled(false);

    QPalette paletteLineEdit = ui->findLineEdit->palette();
    paletteLineEdit.setColor(QPalette::PlaceholderText, QColor("#9AA0A6"));
    ui->findLineEdit->setPalette(paletteLineEdit);
    ui->findLineEdit->setEnabled(false);
    ui->findLineEdit->setPlaceholderText("under construction");
    ui->mainWorkRightStackedWidget->setCurrentIndex(0);

    refillChatListModelWithData(true);
  }
  auto *lv = ui->mainWorkTabChatsList->findChild<QListView *>("chatListView");

  if (lv) {
    if (auto *m = lv->model(); m && m->rowCount() > 0) {
      const QModelIndex first = m->index(0, 0);
      lv->setCurrentIndex(first);
    }
  }
}

void ScreenMainWork::sendMessageCommmand(const QModelIndex idx,
                                         const std::size_t currentChatId,
                                         const QString &newMessageText) {
  if (newMessageText.trimmed().isEmpty())
    return;

  auto chat_ptr = _sessionPtr->getInstance().getChatById(currentChatId);

  std::vector<std::shared_ptr<User>> recipients;
  for (const auto &participant : chat_ptr->getParticipants()) {
    auto user_ptr = participant._user.lock();
    if (user_ptr && user_ptr != _sessionPtr->getActiveUserCl())
      recipients.push_back(user_ptr);
  }

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
    if (auto ed = s->getScreenChattingNewMessageTextEdit()) ed->setText("");
  } else {
    ui->mainWorkPageChatting->getScreenChattingNewMessageTextEdit()->setText("");
  }

         // заменили в модели время последнего сообщения
  _ChatListModel->setLastTime(idx.row(), newMessageTimeStamp);
}

void ScreenMainWork::resetCountUnreadMessagesCommmand()
{
// так как пользователь вошел в чат и все прочитал, сбрасываем количество новых сообщений


         //взяли индекс строки
const auto& idx = ui->mainWorkTabChatsList->getCcurrentChatIndex();

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

void ScreenMainWork::on_createNewChatPushButton_clicked() {

  // экран «чаты контакта»
  const auto chatListInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatList *>(
      "ScreenUserDataChatsListWidget");
  if (!chatListInUser)
    return;

  // указатель на объект содержащий окно сообщний в режиме чата отдельного контакта адресной книги
  auto MessageChattingInUser = ui->mainWorkPageUserDataView->findChild<ScreenChatting *>(
      "ScreenUserDataMessagesListWidget");
  if (!MessageChattingInUser)
    return;

  chatListInUser->setEnabled(false);
  chatListInUser->setVisible(false);

  MessageChattingInUser->setEnabled(false);
  MessageChattingInUser->setVisible(false);

  ui->createNewChatPushButton->setEnabled(false);

  ui->mainWorkChatUserTabWidget->setTabEnabled(0, false);

  ui->addUserToChatPushButton->setVisible(true);

  emit signalStartNewChat();
}

void ScreenMainWork::on_addUserToChatPushButton_clicked() {

  const auto &selectModel = ui->mainWorkUsersList->selectionModel();
  const auto &idx = selectModel->currentIndex();

  if (idx.isValid()) {

    const auto &isActive = idx.data(UserListModel::IsActiveRole).toBool();
    const auto &bunTo = idx.data(UserListModel::BunUntilRole).toLongLong();

    if (!isActive) {
      QMessageBox::warning(this, "Сообщение", "Контакт заблокирован.");
      return;
    }
    if (bunTo != 0 && bunTo > getCurrentDateTimeInt()) {
      QMessageBox::warning(this, "Сообщение", "Пользователь забанен.");
      return;
    }

    const auto &value = idx.data(UserListModel::LoginRole).toString();
    emit signalAddContactToNewChat(value);
  }
}

void ScreenMainWork::on_findLineEdit_textChanged(const QString &arg1) {

  slotFindContactsByPart();
}

void ScreenMainWork::on_findPushButton_clicked() {
  slotFindContactsByPart();
}
