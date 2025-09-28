#pragma once

#include "dto/dto_struct.h"
#include "sql_server.h"

class ServerSession;

class UserAccountUpdateProcessor {
 public:
  explicit UserAccountUpdateProcessor(SQLRequests &sql_requests);

  bool Process(ServerSession &session, PacketListDTO &packet_list,
               const RequestType &request_type, int connection);

 private:
  bool ChangeUserData(ServerSession &session, const UserDTO &user_dto);

  SQLRequests &sql_requests_;
};
