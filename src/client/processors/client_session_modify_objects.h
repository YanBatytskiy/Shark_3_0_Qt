#pragma once

#include <memory>

class ClientSession;
class ClientRequestExecutor;
struct UserDTO;

class ClientSessionModifyObjects {
 public:
  ClientSessionModifyObjects(ClientSession &session,
                             ClientRequestExecutor &request_executor);

  bool changeUserDataProcessing(const UserDTO &user_dto);

 private:
  ClientSession &session_;
  ClientRequestExecutor &request_executor_;
};
