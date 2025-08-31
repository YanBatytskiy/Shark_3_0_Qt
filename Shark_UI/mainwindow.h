#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qt_session/qt_session.h"
#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(std::shared_ptr<QtSession> sessionPtr, QWidget *parent = nullptr);
  ~MainWindow();
  static MainWindow *createSession(std::shared_ptr<QtSession> sessionPtr);
  static int kInstanceCount;

private:
  Ui::MainWindow *ui;
  std::shared_ptr<QtSession> _sessionPtr;
};
#endif // MAINWINDOW_H
