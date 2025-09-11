#include "message_text_browser.h"

MessageTextBrowser::MessageTextBrowser(QWidget* parent) : QTextBrowser(parent) {
  setReadOnly(true);
  setOpenLinks(true);
  setFrameShape(QFrame::NoFrame);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

  setStyleSheet(
      "QTextBrowser{"
      " background:#F7EEDD;"
      " border-radius:14px;"
      " padding:10px 12px;"
      "}"
      );
}

void MessageTextBrowser::setMessage(const QString &senderName, const QString &senderLogin, const QString &timeText, const QString &messageText)
{
  QString header =
      "<table width='100%'><tr>"
      "<td style='color:#800020;font-weight:600;'>"
      + senderName.toHtmlEscaped() + " (" + senderLogin.toHtmlEscaped() + ")"
      + "</td>"
        "<td align='right' style='color:#7A7A7A;'>"
      + timeText.toHtmlEscaped() +
      "</td>"
      "</tr></table>";

  QString body =
      "<div style='color:#000; white-space:pre-wrap;'>"
      + messageText.toHtmlEscaped() +
      "</div>";

  setHtml(header + body);
  updateHeight();
}

void MessageTextBrowser::resizeEvent(QResizeEvent *e)
{
  QTextBrowser::resizeEvent(e);
  updateHeight();
}

void MessageTextBrowser::updateHeight()
{
  document()->setTextWidth(viewport()->width());
  int h = int(document()->size().height()) + 2;
  setMinimumHeight(h);
  setMaximumHeight(h);
}
