#include "model_chat_list_itemdelegate.h"
#include <QApplication>
#include <QPainter>
#include <QTextLayout>
#include <QFontMetrics>
#include "system/date_time_utils.h"

namespace {
constexpr int kPaddingX = 12;
constexpr int kPaddingY = 8;
constexpr int kLineSpacing = 2;
}

QSize ChatListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  Q_UNUSED(index);
  const QFont f1 = option.font;                 // первая строка
  QFont f2 = option.font;                       // вторая строка
  const QFontMetrics fm1(f1);
  const QFontMetrics fm2(f2);
  const int line1 = fm1.height();
  const int line2 = fm2.height();
  const int h = kPaddingY + line1 + kLineSpacing + line2 + kPaddingY;
  const int w = option.rect.width();            // ширина от option.rect
  return {w, h};
}

void ChatListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  // 1) Забираем данные строго из ролей модели
  const QString participantsText  = index.data(Qt::UserRole + 1).toString(); // ParticipantsTextRole
  const QString infoLine          = index.data(Qt::UserRole + 2).toString(); // InfoTextRole
  const int unread                = index.data(Qt::UserRole + 3).toInt();    // UnreadCountRole
  const bool isMuted              = index.data(Qt::UserRole + 4).toBool();   // IsMutedRole
  const std::int64_t timeStamp    = index.data(Qt::UserRole + 5).toLongLong();     // LastTimeRole

  QStyleOptionViewItem opt(option);
  opt.text.clear();
  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, nullptr);


  QRect contentRect = option.rect.adjusted(kPaddingX, kPaddingY, -kPaddingX, -kPaddingY);

  const QFont baseFont = option.font;
  const QFontMetrics baseMetrics(baseFont);

  painter->save();
  painter->setFont(baseFont);
  painter->setPen(option.palette.color(QPalette::Text));

  // Первая строка: список участников
  const QString participantsLine1 = baseMetrics.elidedText(participantsText,Qt::ElideRight, contentRect.width());
  painter->drawText(contentRect, Qt::AlignLeft | Qt::AlignVCenter, participantsLine1);

         // Вторая строка
  const int secondLineY = contentRect.y() + baseMetrics.height() + kLineSpacing;
  QRect secondLineRect(contentRect.x(), secondLineY, contentRect.width(), baseMetrics.height());

  const QString secondLine = baseMetrics.elidedText(infoLine, Qt::ElideRight, secondLineRect.width());
  painter->drawText(secondLineRect, Qt::AlignLeft | Qt::AlignVCenter, secondLine);

  painter->restore();
}

ChatListItemDelegate::ChatListItemDelegate() {}
