#ifndef CHATMESSAGESSCREEN_H
#define CHATMESSAGESSCREEN_H

#include "client_session.h"
#include "models/model_chat_messages.h"
#include <QAbstractItemView>
#include <QWidget>

namespace Ui {
class ScreenChatting;
}

class ScreenChatting : public QWidget {
  Q_OBJECT

public:
  explicit ScreenChatting(QWidget *parent = nullptr);
  ~ScreenChatting();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);
  void setModel(QAbstractItemModel *messageModel);
  QModelIndex currentIndex() const;

public slots:
  void onChatCurrentChanged(const QModelIndex &current,
                            const QModelIndex &previous);

private:
  Ui::ScreenChatting *ui;
  std::shared_ptr<ClientSession> _sessionPtr;
  MessageModel *_messageModel;
  size_t _currentChatId{0};
};

#endif // CHATMESSAGESSCREEN_H
