#pragma once

#include <memory>

#include "client/client_request_executor.h"
#include "dto_struct.h"

class ClientSession;

class ClientSessionModifyObjects {
 public:
  ClientSessionModifyObjects(ClientSession &session,
                             ClientRequestExecutor &request_executor);

  bool changeUserDataProcessing(const UserDTO &user_dto);

 private:
  ClientSession &session_;
  ClientRequestExecutor &request_executor_;
};
