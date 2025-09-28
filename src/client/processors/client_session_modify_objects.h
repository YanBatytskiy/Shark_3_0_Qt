#pragma once

#include <memory>

class ClientSession;
struct UserDTO;

class ClientSessionModifyObjects {
 public:
  explicit ClientSessionModifyObjects(ClientSession &session);

  bool changeUserDataProcessing(const UserDTO &user_dto);

 private:
  ClientSession &session_;
};
