#include "model_chat_list_delegate.h"
#include <QApplication>
#include <QPainter>
#include <QTextLayout>
#include <QFontMetrics>
#include "system/date_time_utils.h"

namespace {
constexpr int kPaddingX = 12;
constexpr int kPaddingY = 6;
constexpr int kLineSpacing = 2;

// Палитра
const QColor kBg      ("#FFFACD"); // обычный фон
const QColor kHoverBg ("#F5F7FA"); // фон под мышью
const QColor kSelBg   ("#40A7E3"); // фон выделения
const QColor kSep     ("#E6E9EF"); // разделитель снизу
const QColor kText1   ("#22262A"); // первая строка
const QColor kText2   ("#6B7C93"); // вторая строка
const QColor kSelText ("#FFFFFF"); // текст на выделении

}

QSize ChatListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  Q_UNUSED(index);

  QFont f1 = option.font;
  QFont f2 = option.font; f2.setPointSizeF(f2.pointSizeF() - 1);
  const int h = kPaddingY + QFontMetrics(f1).height() + kLineSpacing + QFontMetrics(f2).height() + kPaddingY;
  return { option.rect.width(), h };
}

void ChatListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  // 1) Забираем данные строго из ролей модели
  const QString participantsText  = index.data(Qt::UserRole + 1).toString(); // ParticipantsTextRole
  const QString infoLine          = index.data(Qt::UserRole + 2).toString(); // InfoTextRole
  const int unread                = index.data(Qt::UserRole + 3).toInt();    // UnreadCountRole
  const bool isMuted              = index.data(Qt::UserRole + 4).toBool();   // IsMutedRole
  const std::int64_t timeStamp    = index.data(Qt::UserRole + 5).toLongLong();     // LastTimeRole

  const bool selected = option.state & QStyle::State_Selected;
  const bool hover    = option.state & QStyle::State_MouseOver;

  QStyleOptionViewItem opt(option);
  opt.text.clear();

  painter->save();
  painter->setRenderHint(QPainter::TextAntialiasing, true);

  // фон
  if (selected)
    painter->fillRect(option.rect, kSelBg);
  else if (hover)
    painter->fillRect(option.rect, kHoverBg);
  else
    painter->fillRect(option.rect, kBg);

  // рабочая область
  QRect contentRect = option.rect.adjusted(kPaddingX, kPaddingY, -kPaddingX, -kPaddingY);

  // строки
  QFont fontLine1 = option.font;
  QFont fontLine2 = option.font;
  fontLine2.setPointSizeF(fontLine2.pointSizeF() - 1);

  QColor colorLine1 = selected ? kSelText : kText1;
  QColor colorLine2 = selected ? kSelText.lighter(120) : kText2;


  const int h1 = QFontMetrics(fontLine1).height();
  QString line1 = QFontMetrics(fontLine1).elidedText(participantsText,
                                                     Qt::ElideRight,
                                                     contentRect.width());

  painter->setFont(fontLine1);
  painter->setPen(colorLine1);
  painter->drawText(QRect(contentRect.left(), contentRect.top(),
                          contentRect.width(), h1),
                    Qt::AlignLeft | Qt::AlignVCenter, line1);

  const int y2 = contentRect.top() + h1 + kLineSpacing;
  QString line2 = QFontMetrics(fontLine2).elidedText(infoLine,
                                                     Qt::ElideRight,
                                                     contentRect.width());
  painter->setFont(fontLine2);
  painter->setPen(colorLine2);
  painter->drawText(QRect(contentRect.left(), y2,
                          contentRect.width(), QFontMetrics(fontLine2).height()),
                    Qt::AlignLeft | Qt::AlignVCenter, line2);

         // разделитель
  painter->setPen(QPen(kSep));
  painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

  painter->restore();}

ChatListItemDelegate::ChatListItemDelegate() {}
