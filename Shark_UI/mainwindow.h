#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "client_session.h"
#include "model_chat_list.h"
#include "model_user_list.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(std::shared_ptr<ClientSession> sessionPtr, QWidget *parent = nullptr);
  ~MainWindow();
  static MainWindow *createSession(std::shared_ptr<ClientSession> sessionPtr);
  static int kInstanceCount;
  void fillChatListModelWithData();
  void fillUserListModelWithData();

signals:
  void systemDataChanged(const QString &data);

public slots:
  void onConnectionStatusChanged(bool connectionStatus, ServerConnectionMode mode);

private slots:
  void on_exitAction_triggered();

  void on_chatUserTabWidget_currentChanged(int index);

private:
  Ui::MainWindow *ui;
  std::shared_ptr<ClientSession> _sessionPtr;
  ChatListModel* _ChatListModel;
  UserListModel* _userListModel;

};
#endif // MAINWINDOW_H
