#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "qt_session/qt_session.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(std::shared_ptr<Qt_Session> sessionPtr = nullptr, QWidget *parent = nullptr);
  ~MainWindow();
  static MainWindow* createSession(std::shared_ptr<Qt_Session> sessionPtr = nullptr);
  static int kInstanceCount;

private:
  Ui::MainWindow *ui;
  std::shared_ptr<Qt_Session> _sessionPtr;
};
#endif // MAINWINDOW_H
