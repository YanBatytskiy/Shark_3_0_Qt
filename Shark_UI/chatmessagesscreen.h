#ifndef CHATMESSAGESSCREEN_H
#define CHATMESSAGESSCREEN_H

#include <QWidget>
#include "client_session.h"
#include <QAbstractItemView>
#include "model_chat_messages.h"


namespace Ui {
class chatMessagesScreen;
}

class chatMessagesScreen : public QWidget {
  Q_OBJECT

public:
  explicit chatMessagesScreen(QWidget *parent = nullptr);
  ~chatMessagesScreen();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);
  void setModel(QAbstractItemModel* messageModel);
  QModelIndex currentIndex() const;

public slots:
  void onChatCurrentChanged(const QModelIndex& current, const QModelIndex& previous);

private:
  Ui::chatMessagesScreen *ui;
  std::shared_ptr<ClientSession> _sessionPtr;
  MessageModel* _messageModel;
  size_t _currentChatId{0};
};

#endif // CHATMESSAGESSCREEN_H
