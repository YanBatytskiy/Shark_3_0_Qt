#ifndef SCREEN_USER_DATA_H
#define SCREEN_USER_DATA_H

#include <QWidget>
#include "client_session.h"

namespace Ui {
class ScreenUserData;
}

class ScreenUserData : public QWidget {
  Q_OBJECT

public:
  explicit ScreenUserData(QWidget *parent = nullptr);
  ~ScreenUserData();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr);

  void slotClearUserDataToLabels();

  void setUserDataToLabels(const QModelIndex& index);

private:
  Ui::ScreenUserData *ui;
  std::shared_ptr<ClientSession> _sessionPtr;

};

#endif // SCREEN_USER_DATA_H
