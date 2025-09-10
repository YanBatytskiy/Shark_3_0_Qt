#ifndef MODEL_CHAT_MESS_DELEGATE_H
#define MODEL_CHAT_MESS_DELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class model_chat_mess_delegate final : public QStyledItemDelegate {
  Q_OBJECT
public:

  using QStyledItemDelegate::QStyledItemDelegate;

  // explicit model_chat_mess_delegate(QObject *parent = nullptr);

  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

  model_chat_mess_delegate();

};

#endif // MODEL_CHAT_MESS_DELEGATE_H
