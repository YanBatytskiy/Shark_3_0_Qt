#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "client_session.h"
#include "logger.h"
#include "screen_logger_form.h"
#include <QWidget>

class ConnectionMonitor;

namespace Ui {
class ScreenLogin;
}

class ScreenLogin : public QWidget {
  Q_OBJECT
public:
  explicit ScreenLogin(QWidget *parent = nullptr);
  ~ScreenLogin();
  void setDatabase(std::shared_ptr<ClientSession> sessionPtr, std::shared_ptr<Logger> loggerPtr);
  void clearFields();

signals:
  void registrationRequested();
  void accepted(QString login);
  void rejected();

public slots:
  void onConnectionStatusChanged(bool connectionStatus,
                                 ServerConnectionMode mode);

  void slot_show_logger_form(const std::multimap<qint64, QString> &logger_model);

private slots:
  void on_registerModeButton_clicked();
  void on_loginButtonBox_accepted();
  void on_loginButtonBox_rejected();

  void checkSignIn();

  void on_loginEdit_returnPressed();
  void on_passwordEdit_returnPressed();

  void on_baseReInitialisationPushButton_clicked();

  void slotOn_logFileClearPushButton_clicked();
  void slotOn_lookLogSeveralLinePushButton_clicked();
  void slotOn_lookLogLastLinePushButton_clicked();

private:
  Ui::ScreenLogin *ui;
  ScreenLoggerForm *logger_form_{nullptr};
  std::shared_ptr<ClientSession> _sessionPtr;
  std::shared_ptr<Logger> _loggerPtr;
  QString _systemData;
};

#endif // LOGINSCREEN_H
