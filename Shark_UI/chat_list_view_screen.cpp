#include "chat_list_view_screen.h"
#include "ui_chat_list_view_screen.h"
#include "model_chat_list_delegate.h"


ChatListViewScreen::ChatListViewScreen(QWidget *parent) : QWidget(parent), ui(new Ui::ChatListViewScreen) {
  ui->setupUi(this);
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

}

ChatListViewScreen::~ChatListViewScreen()
{
  delete ui;
}

void ChatListViewScreen::setDatabase(std::shared_ptr<ClientSession> sessionPtr)
{
  _sessionPtr = sessionPtr;

}

void ChatListViewScreen::setModel(QAbstractItemModel *chatListModel)
{
  ui->chatListView->setModel(chatListModel);
}

QModelIndex ChatListViewScreen::currentIndex() const {
  return ui->chatListView->currentIndex();
}
