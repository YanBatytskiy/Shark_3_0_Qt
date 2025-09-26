#ifndef SCREEN_LOGGER_FORM_H
#define SCREEN_LOGGER_FORM_H

#include <QStandardItemModel>
#include <QWidget>

#include "client_session.h"
#include "logger.h"

namespace Ui {
class ScreenLoggerForm;
}

class ScreenLoggerForm : public QWidget {
  Q_OBJECT

public:
  explicit ScreenLoggerForm(std::shared_ptr<ClientSession> sessionPtr, std::shared_ptr<Logger> loggerPtr, QWidget *parent = nullptr);
  ~ScreenLoggerForm();

public slots:
  void slot_fill_logger_model(const std::multimap<qint64, QString> &logger_data);

  // protected:
  //   void ShowEvent(QShowEvent *event);

private slots:
  void on_ExitPushButton_clicked();

private:
  Ui::ScreenLoggerForm *ui;
  QStandardItemModel *model_logger_{nullptr};
  std::shared_ptr<Logger> _loggerPtr;
  std::shared_ptr<ClientSession> _sessionPtr;
};

#endif // SCREEN_LOGGER_FORM_H
