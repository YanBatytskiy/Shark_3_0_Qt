#include "models/model_chat_list_delegate.h"

#include "screen_chat_list.h"
#include "ui_screen_chat_list.h"

#include <QItemSelectionModel>
#include <QObject>

ScreenChatList::ScreenChatList(QWidget *parent)
    : QWidget(parent), ui(new Ui::ScreenChatList) {
  ui->setupUi(this);
  // Назначает делегат отрисовки элементов списка.
  ui->chatListView->setItemDelegate(new ChatListItemDelegate(ui->chatListView));

  // Отключает “одинаковую высоту для всех строк
  ui->chatListView->setUniformItemSizes(false); // высоту задаёт делегат

  // Убирает стандартный промежуток между строками
  ui->chatListView->setSpacing(0); // разделитель рисуем сами

  // Заставляет прокрутку работать по пикселям, а не по строкам.
  ui->chatListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

  // Даёт виджету события движения мыши даже без нажатия кнопок.
  ui->chatListView->setMouseTracking(true);

  ui->chatListView->setStyleSheet("QListView { background-color: #FFFFF0; }");
}

ScreenChatList::~ScreenChatList() { delete ui; }

void ScreenChatList::setDatabase(std::shared_ptr<ClientSession> sessionPtr) {
  _sessionPtr = sessionPtr;
}

void ScreenChatList::setModel(ChatListModel *chatListModel) {
  ui->chatListView->setModel(chatListModel);

  auto selectMode = ui->chatListView->selectionModel();

  QObject::connect(selectMode, &QItemSelectionModel::currentChanged, this,
                   &ScreenChatList::currentChatIndexChanged);

  if (chatListModel && chatListModel->rowCount() > 0) {
    ui->chatListView->setCurrentIndex(chatListModel->index(0, 0));
  }
}

QModelIndex ScreenChatList::currentIndex() const {
  return ui->chatListView->currentIndex();
}
