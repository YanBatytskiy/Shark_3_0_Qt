#include "user_object_creation_processor.h"

#include "exceptions_cpp/network_exception.h"
#include "server_session.h"

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

UserObjectCreationProcessor::UserObjectCreationProcessor(
    SQLRequests &sql_requests)
    : sql_requests_(sql_requests) {}

bool UserObjectCreationProcessor::Process(ServerSession &session,
                                          PacketListDTO &packet_list,
                                          const RequestType &request_type,
                                          int connection) {
  PacketListDTO response_packet_list;
  response_packet_list.packets.clear();

  try {
    switch (request_type) {
      case RequestType::RqFrClientCreateUser: {
        if (packet_list.packets.empty() ||
            packet_list.packets[0].structDTOClassType !=
                StructDTOClassType::userDTO) {
          throw exc::EmptyPacketException();
        }

        PacketDTO response_packet;
        response_packet.requestType = request_type;
        response_packet.structDTOClassType = StructDTOClassType::responceDTO;
        response_packet.reqDirection = RequestDirection::ClientToSrv;

        const auto &user_packet =
            static_cast<const StructDTOClass<UserDTO> &>(
                *packet_list.packets[0].structDTOPtr)
                .getStructDTOClass();

        ResponceDTO responce_dto;
        responce_dto.anyNumber = 0;
        responce_dto.anyString = "";

        responce_dto.reqResult = sql_requests_.createUserSQL(
            session.getPGConnection(), user_packet);

        response_packet.structDTOPtr =
            std::make_shared<StructDTOClass<ResponceDTO>>(responce_dto);
        response_packet_list.packets.push_back(response_packet);
        break;
      }
      case RequestType::RqFrClientCreateChat: {
        if (packet_list.packets.size() < 2) {
          throw exc::EmptyPacketException();
        }

        PacketDTO response_packet;
        response_packet.requestType = request_type;
        response_packet.structDTOClassType = StructDTOClassType::responceDTO;
        response_packet.reqDirection = RequestDirection::ClientToSrv;

        const auto &chat_packet =
            static_cast<const StructDTOClass<ChatDTO> &>(
                *packet_list.packets[0].structDTOPtr)
                .getStructDTOClass();

        auto &message_packet = static_cast<StructDTOClass<MessageChatDTO> &>(
            *packet_list.packets[1].structDTOPtr)
                                             .getStructDTOClass();

        ResponceDTO responce_dto;
        responce_dto.anyNumber = 0;
        responce_dto.anyString = "";

        const auto &result = sql_requests_.createChatAndMessageSQL(
            session.getPGConnection(), chat_packet, message_packet);

        if (!result.empty()) {
          responce_dto.reqResult = true;
          responce_dto.anyNumber =
              static_cast<std::size_t>(std::stoull(result[1]));
          responce_dto.anyString = result[0];

          response_packet.structDTOPtr =
              std::make_shared<StructDTOClass<ResponceDTO>>(responce_dto);
          response_packet_list.packets.push_back(response_packet);
        } else {
          responce_dto.reqResult = false;
        }
        break;
      }
      case RequestType::RqFrClientCreateMessage: {
        if (packet_list.packets.empty()) {
          throw exc::EmptyPacketException();
        }

        PacketDTO response_packet;
        response_packet.requestType = request_type;
        response_packet.structDTOClassType = StructDTOClassType::responceDTO;
        response_packet.reqDirection = RequestDirection::ClientToSrv;

        ResponceDTO responce_dto;
        responce_dto.anyNumber = 0;

        const auto &message_packet =
            static_cast<const StructDTOClass<MessageDTO> &>(
                *packet_list.packets[0].structDTOPtr)
                .getStructDTOClass();

        const auto &result = sql_requests_.createMessageSQL(
            session.getPGConnection(), message_packet);

        if (result) {
          responce_dto.reqResult = true;
          responce_dto.anyString = std::to_string(result);

          response_packet.structDTOPtr =
              std::make_shared<StructDTOClass<ResponceDTO>>(responce_dto);
          response_packet_list.packets.push_back(response_packet);
        } else {
          responce_dto.reqResult = false;
          responce_dto.anyString = "0";
        }
        break;
      }
      default:
        throw exc::HeaderWrongTypeException();
    }

    if (!response_packet_list.packets.empty()) {
      session.sendPacketListDTO(response_packet_list, connection);
    }
  } catch (const exc::SendDataException &ex) {
    std::cerr << "Сервер: " << ex.what() << std::endl;
    return false;
  } catch (const exc::NetworkException &ex) {
    std::cerr << "Сервер: " << ex.what() << std::endl;
    return false;
  } catch (const std::bad_cast &ex) {
    std::cerr << "Сервер: Неправильный тип пакета. "
              << ex.what() << std::endl;
    return false;
  }

  return true;
}
