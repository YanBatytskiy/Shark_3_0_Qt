#pragma once

#include <vector>

#include "client/tcp_transport/session_types.h"
#include "dto_struct.h"

class ClientCore;

class ClientRequestExecutor {
 public:
  explicit ClientRequestExecutor(ClientCore &core);

  PacketListDTO processingRequestToServer(std::vector<PacketDTO> &packets,
                                          const RequestType &request_type);

 private:
  ClientCore &core_;
};
