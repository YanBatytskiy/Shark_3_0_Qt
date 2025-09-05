#include "model_chat.h"

ChatModel::ChatModel(QObject *parent) :QAbstractListModel(parent){}

QHash<int, QByteArray> ChatModel::roleNames() const {
  return {
      {ParticipantsListRole, "participantsList"},
      {InfoTextRole, "infoText"},
      {UnreadCountRole, "unreadCount"},
      {LastTimeRole, "lastTime"}
  };
}

int ChatModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid()) return 0;
  return static_cast<int>(_items.size());
}

void ChatModel::appendItem(const ChatItem &chatItem)
{
  const int rows = rowCount();
  beginInsertRows({},rows,rows);
  _items.push_back(chatItem);
  endInsertRows();
}

void ChatModel::removeItem(int row)
{
  if (row <0 || row >= rowCount()) return;

  beginRemoveRows({}, row, row);
  _items.erase(_items.begin() + row);
  endRemoveRows();
}

QVariant ChatModel::data(const QModelIndex &index, int chatRole) const
{
  if (index.isValid() || index.row() < 0 || index.row() >= rowCount()) return {};

  const ChatItem& it = _items[static_cast<size_t>(index.row())];

  switch (chatRole) {
  case ParticipantsListRole: return it.ParticipantsList;
  case InfoTextRole:         return it.InfoText;
  case UnreadCountRole:      return it.UnreadCount;
  case IsMutedRole:          return it.isMuted;
  case LastTimeRole:         return it.LastTime;
  }
  return {};
}

void ChatModel::setParticipantsList(int row, const QString &textValue)
{
  if (row<0 || row >=rowCount()) return;
  _items[static_cast<size_t>(row)].ParticipantsList = textValue;
  const QModelIndex i = index(row);
  emit dataChanged(i, i, {ParticipantsListRole});
}

void ChatModel::setInfoText(int row, const QString &textValue)
{
  if (row <0 || row >=rowCount()) return;
  _items[static_cast<size_t>(row)].InfoText = textValue;
  const QModelIndex i = index(row);
  emit dataChanged(i, i, {InfoTextRole});
}

void ChatModel::setUnreadCount(int row, int newValue)
{
  if (row <0 || row >=rowCount()) return;
  _items[static_cast<size_t>(row)].UnreadCount = newValue;
  const QModelIndex i = index(row);
  emit dataChanged(i, i, {UnreadCountRole});
}

void ChatModel::setIsMuted(int row, bool newValue)
{
  if (row <0 || row >=rowCount()) return;
  _items[static_cast<size_t>(row)].isMuted = newValue;
  const QModelIndex i = index(row);
  emit dataChanged(i, i, {IsMutedRole});
}

void ChatModel::setLastTime(int row, const QDateTime &timeValue)
{
  if (row <0 || row >=rowCount()) return;
  _items[static_cast<size_t>(row )].LastTime = timeValue;
  const QModelIndex i = index(row);
  emit dataChanged(i, i, {LastTimeRole});
}

Qt::ItemFlags ChatModel::flags(const QModelIndex& idx) const {
  if (!idx.isValid()) return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
