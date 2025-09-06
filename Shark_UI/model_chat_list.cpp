#include "model_chat_list.h"

ChatListModel::ChatListModel(QObject *parent) :QAbstractListModel(parent){}

QHash<int, QByteArray> ChatListModel::roleNames() const {
  return {{ParticipantsChatListRole, "participantsList"},
          {InfoTextRole, "infoText"},
          {UnreadCountRole, "unreadCount"},
          {IsMutedRole, "isMuted"},
          {LastTimeRole, "lastTime"}};
}

int ChatListModel::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid()) return 0;
  return static_cast<int>(_items.size());
}

void ChatListModel::appendItem(const ChatListItem &chatItem)
{
  const int rows = rowCount();
  beginInsertRows({},rows,rows);
  _items.push_back(chatItem);
  endInsertRows();
}

QVariant ChatListModel::data(const QModelIndex &index, int chatRole) const
{
  if (!index.isValid() || index.row() < 0 || index.row() >= rowCount()) return {};
  const ChatListItem& it = _items[static_cast<size_t>(index.row())];
  switch (chatRole) {
  case ParticipantsChatListRole: return it.ParticipantsChatList;
  case InfoTextRole:             return it.InfoText;
  case UnreadCountRole:          return it.UnreadCount;
  case IsMutedRole:              return it.isMuted;
  case LastTimeRole:             return static_cast<qlonglong>(it.LastTime);
  }
  return {};
}

void ChatListModel::setParticipantsChatList(int row, const QString &textValue)
{
  if (row<0 || row >=rowCount()) return;
  _items[static_cast<size_t>(row)].ParticipantsChatList = textValue;
  const QModelIndex i = index(row);
  emit dataChanged(i, i, {ParticipantsChatListRole});
}

void ChatListModel::setInfoText(int row, const QString &textValue)
{
  if (row <0 || row >=rowCount()) return;
  _items[static_cast<size_t>(row)].InfoText = textValue;
  const QModelIndex i = index(row);
  emit dataChanged(i, i, {InfoTextRole});
}

void ChatListModel::setUnreadCount(int row, int newValue)
{
  if (row <0 || row >=rowCount()) return;
  _items[static_cast<size_t>(row)].UnreadCount = newValue;
  const QModelIndex i = index(row);
  emit dataChanged(i, i, {UnreadCountRole});
}

void ChatListModel::setIsMuted(int row, bool newValue)
{
  if (row <0 || row >=rowCount()) return;
  _items[static_cast<size_t>(row)].isMuted = newValue;
  const QModelIndex i = index(row);
  emit dataChanged(i, i, {IsMutedRole});
}

void ChatListModel::setLastTime(int row, std::int64_t timeValue)
{
  if (row <0 || row >=rowCount()) return;
  _items[static_cast<size_t>(row )].LastTime = timeValue;
  const QModelIndex i = index(row);
  emit dataChanged(i, i, {LastTimeRole});
}

void ChatListModel::fillChatListItem(const QString& participantsChatList, const QString& infoText, const int& unreadCount, bool isMuted, std::int64_t lastTime)
{
  ChatListItem chatListItem;
  chatListItem.ParticipantsChatList = participantsChatList;
  chatListItem.InfoText = infoText;
  chatListItem.UnreadCount = unreadCount;
  chatListItem.isMuted = isMuted;
  chatListItem.LastTime = lastTime;

  appendItem(chatListItem);

}

Qt::ItemFlags ChatListModel::flags(const QModelIndex& idx) const {
  if (!idx.isValid()) return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
