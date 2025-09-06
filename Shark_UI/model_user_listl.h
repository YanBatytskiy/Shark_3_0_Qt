#ifndef MODEL_USER_LISTL_H
#define MODEL_USER_LISTL_H

#include <QAbstractListModel>
#include <QObject>

class UserListModel : public QAbstractListModel {
  Q_OBJECT

private:
  struct ChatListItem {
    QString Login;
    QString PasswordHash;
    QString InfoText;
    int UnreadCount;
    bool isMuted = false;
    std::int64_t LastTime;
  };
  std::vector<ChatListItem> _items;

public:
  explicit UserListModel(QObject *parent = nullptr);
  QHash<int, QByteArray> UserListModel::roleNames() const override;
};

#endif // MODEL_USER_LISTL_H
