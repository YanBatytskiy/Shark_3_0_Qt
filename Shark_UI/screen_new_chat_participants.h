#ifndef SCREEN_NEW_CHAT_PARTICIPANTS_H
#define SCREEN_NEW_CHAT_PARTICIPANTS_H

#include <QListView>
#include <QStringListModel>
#include <QWidget>

namespace Ui {
class ScreenNewChatParticipants;
}

class ScreenNewChatParticipants : public QWidget {
  Q_OBJECT

public:
  explicit ScreenNewChatParticipants(QWidget *parent = nullptr);
  ~ScreenNewChatParticipants();

signals:
  void signalCancelNewChat();

public slots:
  void slotCollectParticipantsForNewChat();
  void slotAddContactToParticipantsList(const QString &value);

private slots:

  void on_screenNewChatDeleteContactPushButton_clicked();

  void on_screenUserDataCancelNewChatPushButton_clicked();

private:
  Ui::ScreenNewChatParticipants *ui;
  QStringListModel *_participantsListModel{nullptr};
};

#endif // SCREEN_NEW_CHAT_PARTICIPANTS_H
