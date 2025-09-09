#ifndef CHAT_LIST_VIEW_SCREEN_H
#define CHAT_LIST_VIEW_SCREEN_H

#include <QWidget>
#include "client_session.h"
#include "model_chat_list.h"

namespace Ui {
class ChatListViewScreen;
}

class ChatListViewScreen : public QWidget {
  Q_OBJECT
public:
  explicit ChatListViewScreen(QWidget *parent = nullptr);
  ~ChatListViewScreen();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);
  void setModel(QAbstractItemModel* chatListModel);

  QModelIndex currentIndex() const;

signals:
    void currentChatIdChanged(std::size_t chatId);

private:
  Ui::ChatListViewScreen *ui;
  std::shared_ptr<ClientSession> _sessionPtr;

  ChatListModel* _ChatListModel;

};

#endif // CHAT_LIST_VIEW_SCREEN_H
