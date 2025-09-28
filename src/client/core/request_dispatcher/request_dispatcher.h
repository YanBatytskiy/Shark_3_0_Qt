#pragma once

#include "dto_struct.h"

#include <atomic>
#include <vector>

class ChatSystem;
class TcpTransport;

class RequestDispatcher {
public:
  RequestDispatcher(ChatSystem &chat_system, TcpTransport &transport);

  PacketListDTO process(int socket_fd, std::atomic_bool &status_online, std::vector<PacketDTO> &packets,
                        const RequestType &request_type);

private:
  ChatSystem &chat_system_;
  TcpTransport &transport_;
};

