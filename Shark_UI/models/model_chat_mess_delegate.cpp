#include "model_chat_mess_delegate.h"
#include <QApplication>
#include <QPainter>
#include <QTextLayout>
#include <QFontMetrics>
#include "system/date_time_utils.h"

// model_chat_mess_delegate::model_chat_mess_delegate(QObject *parent) : QStyledItemDelegate{parent} {}

namespace {
constexpr int kPaddingX = 12;
constexpr int kPaddingY = 6;
constexpr int kLineSpacing = 2;

// Палитра
const QColor kBg      ("DEDDDA"); // обычный фон
const QColor kSep     ("#E6E9EF"); // разделитель снизу
const QColor kText1   ("#800020"); //  строка c именем
const QColor kText2   ("#000000"); //  строка с текстом
}

QSize model_chat_mess_delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  Q_UNUSED(index);

  // QFont f1 = option.font;

  const int h = kPaddingY + QFontMetrics(option.font).height() +
                kLineSpacing + QFontMetrics(option.font).height() +
                kPaddingY;
  return {option.rect.width(), h};
}

void model_chat_mess_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  // 1) Забираем данные строго из ролей модели
  const QString messageText  = index.data(Qt::UserRole + 1).toString();
  const QString senderLogin  = index.data(Qt::UserRole + 2).toString();
  const QString senderName  = index.data(Qt::UserRole + 3).toString();
  const std::int64_t timeStamp  = index.data(Qt::UserRole + 4).toLongLong();
  const std::size_t messageId  = index.data(Qt::UserRole + 5).toLongLong();

  const auto dateTimeStamp = formatTimeStampToString(timeStamp, true);
  const QString firstLine = senderName + " ("+ senderLogin +")          " + QString::fromStdString(dateTimeStamp);

  QStyleOptionViewItem opt(option);
  opt.text.clear();

  painter->save();
  painter->setRenderHint(QPainter::TextAntialiasing, true);

  painter->fillRect(option.rect, kBg);

  // рабочая область
  QRect contentRect = option.rect.adjusted(kPaddingX, kPaddingY, -kPaddingX, -kPaddingY);

  // строки
  QFont fontLine1 = option.font;
  QFont fontLine2 = option.font;

  QColor colorLine1 = kText1;
  QColor colorLine2 = kText2;

  const int h1 = QFontMetrics(fontLine1).height();
  QString line1 = QFontMetrics(fontLine1).elidedText(firstLine,
                                                     Qt::ElideRight,
                                                     contentRect.width());

  painter->setFont(fontLine1);
  painter->setPen(colorLine1);
  painter->drawText(QRect(contentRect.left(), contentRect.top(),
                          contentRect.width(), h1),
                    Qt::AlignLeft | Qt::AlignVCenter, line1);

  const int h2 = QFontMetrics(fontLine2).height();
  QString line2 = QFontMetrics(fontLine2).elidedText(messageText,
                                                     Qt::ElideRight,
                                                     contentRect.width());

  painter->setFont(fontLine2);
  painter->setPen(colorLine2);
  const int y2 = contentRect.top() + h1 + kLineSpacing;
  painter->drawText(QRect(contentRect.left(), y2,
                          contentRect.width(), h2),
                    Qt::AlignLeft | Qt::AlignVCenter, line2);

         // разделитель
  painter->setPen(QPen(kSep));
  painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

  painter->restore();
}

model_chat_mess_delegate::model_chat_mess_delegate() {}
