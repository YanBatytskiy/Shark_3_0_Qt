#include "client/client_request_executor.h"

#include "client/core/client_core.h"

ClientRequestExecutor::ClientRequestExecutor(ClientCore &core) : core_(core) {}

PacketListDTO ClientRequestExecutor::processingRequestToServer(
    std::vector<PacketDTO> &packets, const RequestType &request_type) {
  return core_.processingRequestToServerCore(packets, request_type);
}
