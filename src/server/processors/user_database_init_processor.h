#pragma once

#include "dto/dto_struct.h"
#include "sql_server.h"

class ServerSession;

class UserDatabaseInitProcessor {
 public:
  explicit UserDatabaseInitProcessor(SQLRequests &sql_requests);

  bool Process(ServerSession &session, PacketListDTO &packet_list,
               const RequestType &request_type, int connection);

 private:
  SQLRequests &sql_requests_;
};
