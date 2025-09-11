#include "message/message_content.h"
#include "message/message_content_struct.h"

#include "models/model_chat_list.h"
#include "models/model_chat_mess_delegate.h"
#include "models/model_chat_messages.h"
#include "system/date_time_utils.h"

#include "message_text_browser.h"

#include "screen_chatting.h"
#include "ui_screen_chatting.h"

ScreenChatting::ScreenChatting(QWidget *parent)
    : QWidget(parent), ui(new Ui::ScreenChatting) {
  ui->setupUi(this);


         // Назначает делегат отрисовки элементов списка.
  ui->ChatMessagesListView->setItemDelegate(
      new model_chat_mess_delegate(ui->ChatMessagesListView));

         // Отключает “одинаковую высоту для всех строк
  ui->ChatMessagesListView->setUniformItemSizes(false); // высоту задаёт делегат

         // Убирает стандартный промежуток между строками
  ui->ChatMessagesListView->setSpacing(0); // разделитель рисуем сами

         // Заставляет прокрутку работать по пикселям, а не по строкам.
  ui->ChatMessagesListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

  // Заставляет прокрутку работать по пикселям, а не по строкам.
  ui->ChatMessagesListView->setVerticalScrollMode(
      QAbstractItemView::ScrollPerPixel);
}

ScreenChatting::~ScreenChatting() { delete ui; }

void ScreenChatting::setDatabase(std::shared_ptr<ClientSession> sessionPtr) {
  _sessionPtr = sessionPtr;
}

void ScreenChatting::setModel(MessageModel *messageModel) {
  ui->ChatMessagesListView->setModel(messageModel);
  _messageModel = messageModel;
}

QModelIndex ScreenChatting::currentIndex() const {
  return ui->ChatMessagesListView->currentIndex();
}


void ScreenChatting::onChatCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
  if (!current.isValid()) {
    _currentChatId = 0;
    return;
  }

  Q_UNUSED(previous);

  _currentChatId =
      static_cast<size_t>(current.data(ChatListModel::ChatIdRole).toLongLong());

  emit ChatListIdChanged(_currentChatId);
}
