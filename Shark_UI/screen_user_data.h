#ifndef SCREEN_USER_DATA_H
#define SCREEN_USER_DATA_H

#include <QWidget>
#include <QPushButton>
#include "client_session.h"

namespace Ui {
class ScreenUserData;
}

class ScreenUserData : public QWidget {
  Q_OBJECT

public:
  explicit ScreenUserData(QWidget *parent = nullptr);
  ~ScreenUserData();
  QPushButton* blockPushButton;
  QPushButton* unblockPushButton;
  QPushButton* bunToPushButton;
  QPushButton* unBunToPushButton;
  void setDatabase(std::shared_ptr<ClientSession> client_session_ptr);

  void slotClearUserDataToLabels();

  void setUserDataToLabels(const QModelIndex& index);

signals:
  void signalNewChatListBecameEnabled();
  void signal_on_block_push_button_clicked();
  void signal_on_unblock_push_button_clicked();
  void signal_on_bun_push_button_clicked();
  void signal_on_unbun_push_button_clicked();

private slots:
  void slot_on_block_push_button_clicked();
  void slot_on_unblock_push_button_clicked();
  void slot_on_bun_push_button_clicked();
  void slot_on_unbun_push_button_clicked();

private:
  Ui::ScreenUserData *ui;
  std::shared_ptr<ClientSession> client_session_ptr_;

};

#endif // SCREEN_USER_DATA_H
