#include "request_dispatcher.h"

#include "chat_system/chat_system.h"
#include "client/tcp_transport/tcp_transport.h"

#include "exceptions_qt/exception_network.h"
#include "exceptions_qt/errorbus.h"
#include "system/date_time_utils.h"
#include "system/serialize.h"

#include <QString>

#include <memory>

RequestDispatcher::RequestDispatcher(ChatSystem &chat_system, TcpTransport &transport)
    : chat_system_(chat_system), transport_(transport) {}

PacketListDTO RequestDispatcher::process(int socket_fd, std::atomic_bool &status_online, std::vector<PacketDTO> &packets,
                                         const RequestType &request_type) {
  PacketListDTO send_list;
  PacketListDTO result;
  result.packets.clear();

  if (!status_online.load(std::memory_order_acquire) || socket_fd < 0) {
    const auto time_stamp = formatTimeStampToString(getCurrentDateTimeInt(), true);
    const auto time_stamp_qt = QString::fromStdString(time_stamp);

    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(""),
                                     QStringLiteral("[%1]   [ERROR]   [NETWORK]   processingRequestToServer   no connection with server")
                                         .arg(time_stamp_qt));
    return result;
  }

  try {
    UserLoginPasswordDTO header;
    if (chat_system_.getActiveUser()) {
      header.login = chat_system_.getActiveUser()->getLogin();
    } else {
      header.login = "!";
    }
    header.passwordhash = "UserHeder";

    PacketDTO header_packet;
    header_packet.requestType = request_type;
    header_packet.structDTOClassType = StructDTOClassType::userLoginPasswordDTO;
    header_packet.reqDirection = RequestDirection::ClientToSrv;
    header_packet.structDTOPtr = std::make_shared<StructDTOClass<UserLoginPasswordDTO>>(header);

    send_list.packets.push_back(header_packet);
    for (const auto &packet : packets) {
      send_list.packets.push_back(packet);
    }

    auto payload = serializePacketList(send_list.packets);
    result = transport_.transportPackets(socket_fd, status_online, payload);
  } catch (const exc_qt::LostConnectionException &ex) {
    emit exc_qt::ErrorBus::i().error(QString::fromUtf8(ex.what()),
                                     QStringLiteral("Клиент processingRequestToServer: "));
    result.packets.clear();
  }

  return result;
}
