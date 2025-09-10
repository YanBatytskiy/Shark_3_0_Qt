#ifndef CHAT_LIST_VIEW_SCREEN_H
#define CHAT_LIST_VIEW_SCREEN_H

#include "client_session.h"
#include <QWidget>
#include "model_chat_list.h"
#include <QAbstractItemView>

namespace Ui {
class ScreenChatList;
}

class ScreenChatList : public QWidget {
  Q_OBJECT
public:
  explicit ScreenChatList(QWidget *parent = nullptr);
  ~ScreenChatList();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);
  void setModel(ChatListModel *chatListModel);

  QModelIndex currentIndex() const;

signals:
  void currentChatIndexChanged(const QModelIndex &current,
                               const QModelIndex &previous);

private:
  Ui::ScreenChatList *ui;
  std::shared_ptr<ClientSession> _sessionPtr;

  // ChatListModel* _ChatListModel;
};

#endif // CHAT_LIST_VIEW_SCREEN_H
