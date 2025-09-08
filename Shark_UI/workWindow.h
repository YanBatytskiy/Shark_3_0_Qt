#ifndef WORKWINDOW_H
#define WORKWINDOW_H

#include <QDialog>
#include <memory>
#include "client_session.h"
#include "model_chat_list.h"
#include "model_user_list.h"

namespace Ui {
class work_window;
}

class work_window : public QDialog {
  Q_OBJECT

public:
  explicit work_window(QWidget *parent = nullptr);
  ~work_window();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);

  void fillChatListModelWithData();
  void fillUserListModelWithData();


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

};

#endif // WORKWINDOW_H
