#ifndef MODEL_CHAT_H
#define MODEL_CHAT_H

#include <QObject>
#include <QDateTime>
#include <QAbstractListModel>
#include <vector>


class ChatModel final : public QAbstractListModel{
  Q_OBJECT
private:
  struct ChatItem {
    QString ParticipantsList;
    QString InfoText;
    int UnreadCount;
    bool isMuted = false;
    QDateTime LastTime;
  };
  std::vector<ChatItem> _items;

public:
  enum Roles: int {
    ParticipantsListRole = Qt::UserRole +1,
    InfoTextRole,
    UnreadCountRole,
    IsMutedRole,
    LastTimeRole
  };

  Q_ENUM(Roles)

  explicit ChatModel(QObject *parent = nullptr);

  QHash<int, QByteArray> roleNames() const override;
  int rowCount(const QModelIndex& parent = {}) const override;
  void appendItem(const ChatItem& chatItem);
  void removeItem(int row);
  Qt::ItemFlags flags(const QModelIndex& idx) const override;
  QVariant data(const QModelIndex& index, int chatRole) const override;

  void setParticipantsList(int row, const QString& textValue);
  void setInfoText(int row, const QString& textValue);
  void setUnreadCount(int row, int newValue);
  void setIsMuted(int row, bool newValue);
  void setLastTime(int row, const QDateTime& timeValue);

signals:


};

#endif // MODEL_CHAT_H
