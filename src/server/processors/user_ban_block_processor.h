#pragma once

#include "dto/dto_struct.h"
#include "sql_queries/user_sql_writer.h"

class ServerSession;

class UserBanBlockProcessor {
 public:
  explicit UserBanBlockProcessor(UserSqlWriter &user_sql_writer);

  bool Process(ServerSession &session, PacketListDTO &packet_list,
               const RequestType &request_type, int connection);

 private:
  UserSqlWriter &user_sql_writer_;
};
