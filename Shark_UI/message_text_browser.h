#ifndef MESSAGE_TEXT_BROWSER_H
#define MESSAGE_TEXT_BROWSER_H

#include <QObject>
#include <QTextBrowser>
#include <QWidget>

class MessageTextBrowser : public QTextBrowser {
  Q_OBJECT
public:
explicit  MessageTextBrowser(QWidget* parent = nullptr);
  void setMessage(const QString& senderName,
                  const QString& senderLogin,
                  const QString& timeText,
                  const QString& messageText);

void updateHeight();

private:
  bool _updatingHeight = false;
};

#endif // MESSAGE_TEXT_BROWSER_H
