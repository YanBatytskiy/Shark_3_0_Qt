#include "chatmessagesscreen.h"
#include "ui_chatmessagesscreen.h"
#include "model_chat_mess_delegate.h"
#include "model_chat_list.h"
#include "model_chat_messages.h"
#include "message/message_content.h"
#include "message/message_content_struct.h"


chatMessagesScreen::chatMessagesScreen(QWidget *parent) : QWidget(parent), ui(new Ui::chatMessagesScreen) {
  ui->setupUi(this);
  // Назначает делегат отрисовки элементов списка.
  ui->ChatMessagesListView->setItemDelegate(new model_chat_mess_delegate(ui->ChatMessagesListView));

  // Отключает “одинаковую высоту для всех строк
  ui->ChatMessagesListView->setUniformItemSizes(false);               // высоту задаёт делегат

  //Убирает стандартный промежуток между строками
  ui->ChatMessagesListView->setSpacing(0);                            // разделитель рисуем сами

         //Заставляет прокрутку работать по пикселям, а не по строкам.
  ui->ChatMessagesListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    }

chatMessagesScreen::~chatMessagesScreen() { delete ui; }

void chatMessagesScreen::setDatabase(std::shared_ptr<ClientSession> sessionPtr)
{
  _sessionPtr = sessionPtr;

}

void chatMessagesScreen::setModel(QAbstractItemModel *messageModel)
{
  ui->ChatMessagesListView->setModel(messageModel);
  _messageModel = qobject_cast<MessageModel*>(messageModel);
}

QModelIndex chatMessagesScreen::currentIndex() const
{
  return ui->ChatMessagesListView->currentIndex();
}

void chatMessagesScreen::onChatCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
  if (!current.isValid()) {_currentChatId = 0; return; }

  Q_UNUSED(previous);

  if (!current.isValid()) { _currentChatId = 0; return; }

  _currentChatId = static_cast<size_t>(current.data(ChatListModel::ChatIdRole).toLongLong());

  // очистить старые сообщения
 _messageModel->clear();

         // достать новые
  const auto messages = _sessionPtr->getInstance().getChatById(_currentChatId)->getMessages();
  for (const auto& message : messages) {
    if (!message.second) continue;

    QString messageText;
    const auto& content = message.second->getContent();
    if (!content.empty()) {
      const auto textContent = std::dynamic_pointer_cast<MessageContent<TextContent>>(content[0]);
      if (textContent)
        messageText = QString::fromStdString(textContent->getMessageContent()._text);
    }

    const auto senderPtr = message.second->getSender().lock();
    QString senderLogin = senderPtr ? QString::fromStdString(senderPtr->getLogin()) : "Удален";
    QString senderName  = senderPtr ? QString::fromStdString(senderPtr->getUserName()) : "";

    std::int64_t timeStamp = static_cast<int64_t>(message.first);
    std::size_t messageId  = static_cast<size_t>(message.second->getMessageId());

    _messageModel->fillMessageItem(messageText, senderLogin, senderName, timeStamp, messageId);
  }
}
