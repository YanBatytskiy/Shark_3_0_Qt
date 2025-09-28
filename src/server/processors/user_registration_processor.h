#pragma once

#include "dto/dto_struct.h"
#include "sql_server.h"

#include <optional>
#include <string>
#include <vector>

class ServerSession;

class UserRegistrationProcessor {
 public:
  explicit UserRegistrationProcessor(SQLRequests &sql_requests);

  bool Process(ServerSession &session, PacketListDTO &packet_list,
               const RequestType &request_type, int connection);

 private:
  bool CheckUserLogin(ServerSession &session, const std::string &login);
  bool CheckUserPassword(ServerSession &session,
                         const UserLoginPasswordDTO &credentials);

  SQLRequests &sql_requests_;
};
