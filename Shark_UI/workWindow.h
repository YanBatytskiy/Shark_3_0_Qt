#ifndef WORKWINDOW_H
#define WORKWINDOW_H

#include <QWidget>
#include <memory>
#include "client_session.h"
#include "model_chat_list.h"
#include "model_user_list.h"
#include "model_chat_messages.h"

namespace Ui {
class work_window;
}

class work_window : public QWidget {
  Q_OBJECT

public:
  explicit work_window(QWidget *parent = nullptr);
  ~work_window();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);

  void fillChatListModelWithData();
  void fillUserListModelWithData();
  void fillMessageModelWithData();


public slots:
  void onConnectionStatusChanged(bool connectionStatus, ServerConnectionMode mode);
  void createSession();

private slots:
  void on_chatUserTabWidget_currentChanged(int index);


private:
  Ui::work_window *ui;
  std::shared_ptr<ClientSession> _sessionPtr;

  ChatListModel* _ChatListModel;
  UserListModel* _userListModel;
  MessageModel* _MessageModel;

};

#endif // WORKWINDOW_H
