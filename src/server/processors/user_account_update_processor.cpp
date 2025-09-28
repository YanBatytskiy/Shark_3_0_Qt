#include "user_account_update_processor.h"

#include "exceptions_cpp/network_exception.h"
#include "exceptions_cpp/sql_exception.h"
#include "server_session.h"

#include <cstdlib>
#include <iostream>
#include <libpq-fe.h>
#include <memory>

UserAccountUpdateProcessor::UserAccountUpdateProcessor(
    SQLRequests &sql_requests)
    : sql_requests_(sql_requests) {}

bool UserAccountUpdateProcessor::Process(ServerSession &session,
                                         PacketListDTO &packet_list,
                                         const RequestType &request_type,
                                         int connection) {
  PacketListDTO response_packet_list;
  response_packet_list.packets.clear();

  try {
    switch (request_type) {
      case RequestType::RqFrClientChangeUserData: {
        if (packet_list.packets.empty()) {
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

        if (ChangeUserData(session, user_packet)) {
          responce_dto.reqResult = true;
          responce_dto.anyString = user_packet.login;
        } else {
          responce_dto.reqResult = false;
        }

        response_packet.structDTOPtr =
            std::make_shared<StructDTOClass<ResponceDTO>>(responce_dto);
        response_packet_list.packets.push_back(response_packet);
        break;
      }
      case RequestType::RqFrClientChangeUserPassword:
        // TODO: реализовать смену пароля клиента.
        break;
      default:
        break;
    }

    if (!response_packet_list.packets.empty()) {
      session.sendPacketListDTO(response_packet_list, connection);
    }
  } catch (const exc::SendDataException &ex) {
    std::cerr << "Сервер: " << ex.what() << std::endl;
    return false;
  } catch (const exc::EmptyPacketException &ex) {
    std::cerr << "Сервер: " << ex.what() << std::endl;
    return false;
  } catch (const exc::NetworkException &ex) {
    std::cerr << "Сервер: " << ex.what() << std::endl;
    return false;
  }

  return true;
}

bool UserAccountUpdateProcessor::ChangeUserData(ServerSession &session,
                                                const UserDTO &user_dto) {
  PGresult *result = nullptr;

  std::string sql;
  std::string login = user_dto.login;
  std::string name = user_dto.userName;
  std::string email = user_dto.email;
  std::string phone = user_dto.phone;

  try {
    for (std::size_t pos = 0;
         (pos = login.find('\'', pos)) != std::string::npos; pos += 2) {
      login.replace(pos, 1, "''");
    }
    for (std::size_t pos = 0;
         (pos = name.find('\'', pos)) != std::string::npos; pos += 2) {
      name.replace(pos, 1, "''");
    }
    for (std::size_t pos = 0;
         (pos = email.find('\'', pos)) != std::string::npos; pos += 2) {
      email.replace(pos, 1, "''");
    }
    for (std::size_t pos = 0;
         (pos = phone.find('\'', pos)) != std::string::npos; pos += 2) {
      phone.replace(pos, 1, "''");
    }

    sql = "UPDATE public.users SET name = '" + name + "', email = '" + email +
          "', phone = '" + phone + "' WHERE login = '" + login + "';";

    result = sql_requests_.execSQL(session.getPGConnection(), sql);

    if (result == nullptr) {
      throw exc::SQLSelectException(
          ", UserAccountUpdateProcessor::ChangeUserData");
    }

    const char *tuples = PQcmdTuples(result);
    const long affected_rows =
        tuples != nullptr ? std::strtol(tuples, nullptr, 10) : 0;

    PQclear(result);

    if (affected_rows == 0) {
      return false;
    }

    return true;
  } catch (const exc::SQLSelectException &ex) {
    std::cerr << "Сервер: " << ex.what() << std::endl;
    if (result != nullptr) {
      PQclear(result);
    }
    return false;
  }
}
