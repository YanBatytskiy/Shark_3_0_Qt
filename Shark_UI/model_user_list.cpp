#include "model_user_listl.h"

UserListModel::UserListModel(QObject *parent) : QAbstractListModel{parent} {}

QHash<int, QByteArray> UserListModel::roleNames() const {
  return {{ParticipantsChatListRole, "participantsList"},
          {InfoTextRole, "infoText"},
          {UnreadCountRole, "unreadCount"},
          {IsMutedRole, "isMuted"},
          {LastTimeRole, "lastTime"}};
}
