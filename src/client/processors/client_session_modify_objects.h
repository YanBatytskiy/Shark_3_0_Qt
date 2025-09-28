#pragma once

#include <memory>

class ClientCore;
struct UserDTO;

class ClientSessionModifyObjects {
public:
  explicit ClientSessionModifyObjects(ClientCore &core);

  bool changeUserDataCl(const UserDTO &user_dto);

private:
  ClientCore &core_;
};
