#ifndef WORKWINDOW_H
#define WORKWINDOW_H

#include "client_session.h"

#include "models/model_chat_list.h"
#include "models/model_chat_messages.h"
#include "models/model_user_list.h"

#include <QWidget>
#include <memory>

namespace Ui {
class ScreenMainWork;
}

class ScreenMainWork : public QWidget {
  Q_OBJECT

public:
  explicit ScreenMainWork(QWidget *parent = nullptr);
  ~ScreenMainWork();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);

  void fillChatListModelWithData(bool allChats);

  void fillUserListModelWithData();
  void fillMessageModelWithData(std::size_t chatId);
  void createSession();

  void setupUserList();
  void setupScreenChatting();

  void sendMessageCommmand();
  void resetCountUnreadMessagesCommmand();

signals:
  void currentUserIndexChanged(const QModelIndex &current);

public slots:
  void onConnectionStatusChanged(bool connectionStatus,
                                 ServerConnectionMode mode);

private slots:
  void on_chatUserTabWidget_currentChanged(int index);

private:
  Ui::ScreenMainWork *ui;
  std::shared_ptr<ClientSession> _sessionPtr;

  ChatListModel *_ChatListModel;
  UserListModel *_userListModel;
  MessageModel *_MessageModel;
};

#endif // WORKWINDOW_H
