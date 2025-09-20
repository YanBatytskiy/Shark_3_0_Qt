#ifndef SCREEN_USER_PROFILE_H
#define SCREEN_USER_PROFILE_H

#include <QWidget>

namespace Ui {
class ScreenUserProfile;
}

class ScreenUserProfile : public QWidget {
  Q_OBJECT

public:
  explicit ScreenUserProfile(QWidget *parent = nullptr);
  ~ScreenUserProfile();

signals:
  void signalCloseUserProfile();

private slots:
  void on_cancelPushButton_clicked();

  void on_savePushButton_clicked();

  void on_nameLineEdit_textChanged(const QString &arg1);

  void on_emailLineEdit_textChanged(const QString &arg1);

  void on_phoneLineEdit_textChanged(const QString &arg1);

  void on_confirnPasswordLineEdit_textChanged(const QString &arg1);

private:
  Ui::ScreenUserProfile *ui;
  bool _isDataChanged{false};
};

#endif // SCREEN_USER_PROFILE_H
