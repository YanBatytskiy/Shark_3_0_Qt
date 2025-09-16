#ifndef WORKWINDOW_H
#define WORKWINDOW_H

#include "client_session.h"

#include "models/model_chat_list.h"
#include "models/model_chat_messages.h"
#include "models/model_user_list.h"

#include <QButtonGroup>
#include <QMessageBox>
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
  void clearMessageModelWithData();

  void createSession();

  void setupUserList();
  void setupScreenChatting();

  void clearChatListModelWithData();

  void refillChatListModelWithData(bool allChats);

  void sendMessageCommmand(const QModelIndex idx,
                           const std::size_t currentChatId,
                           const QString &newMessageText);
  void resetCountUnreadMessagesCommmand();

signals:
  void signalStartNewChat();
  void signalAddContactToNewChat(const QString &value);
  void signalClearUserDataToLabels();

public slots:
  void onConnectionStatusChanged(bool connectionStatus,
                                 ServerConnectionMode mode);
  void slotCancelNewChat();

  void slotFindContactsByPart();

private slots:
  void on_mainWorkChatUserTabWidget_currentChanged(int index);

  void on_createNewChatPushButton_clicked();

  void on_addUserToChatPushButton_clicked();

  void on_findLineEdit_textChanged(const QString &arg1);

  void on_findPushButton_clicked();

  // void on_mainWorkUsersList_clicked(const QModelIndex &index);

  void on_findLineEdit_editingFinished();

private:
  Ui::ScreenMainWork *ui;
  std::shared_ptr<ClientSession> _sessionPtr;

  QButtonGroup *_findRadoButtonGroup{nullptr};
  ChatListModel *_ChatListModel;
  UserListModel *_userListModel;
  MessageModel *_MessageModel;

  bool _startFind{true};
};

#endif // WORKWINDOW_H
