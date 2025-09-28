#include "user_registration_processor.h"

#include "exceptions_cpp/network_exception.h"
#include "exceptions_cpp/sql_exception.h"
#include "server_session.h"

#include <iostream>
#include <libpq-fe.h>
#include <memory>
#include <typeinfo>

UserRegistrationProcessor::UserRegistrationProcessor(SQLRequests &sql_requests)
    : sql_requests_(sql_requests) {}

bool UserRegistrationProcessor::Process(ServerSession &session,
                                        PacketListDTO &packet_list,
                                        const RequestType &request_type,
                                        int connection) {
  PacketListDTO response_packet_list;
  response_packet_list.packets.clear();

  try {
    switch (request_type) {
      case RequestType::RqFrClientCheckLogin: {
        if (packet_list.packets.empty()) {
          throw exc::EmptyPacketException();
        }

        PacketDTO response_packet;
        response_packet.requestType = request_type;
        response_packet.structDTOClassType = StructDTOClassType::responceDTO;
        response_packet.reqDirection = RequestDirection::ClientToSrv;

        const auto &login_packet =
            static_cast<const StructDTOClass<UserLoginDTO> &>(
                *packet_list.packets[0].structDTOPtr)
                .getStructDTOClass();

        ResponceDTO responce_dto;
        responce_dto.anyNumber = 0;

        if (CheckUserLogin(session, login_packet.login)) {
          responce_dto.reqResult = true;
          responce_dto.anyString = login_packet.login;
        } else {
          responce_dto.reqResult = false;
          responce_dto.anyString = "@";
        }

        response_packet.structDTOPtr =
            std::make_shared<StructDTOClass<ResponceDTO>>(responce_dto);
        response_packet_list.packets.push_back(response_packet);
        break;
      }
      case RequestType::RqFrClientCheckLogPassword: {
        if (packet_list.packets.empty()) {
          throw exc::EmptyPacketException();
        }

        PacketDTO response_packet;
        response_packet.requestType = request_type;
        response_packet.structDTOClassType = StructDTOClassType::responceDTO;
        response_packet.reqDirection = RequestDirection::ClientToSrv;

        const auto &credentials =
            static_cast<const StructDTOClass<UserLoginPasswordDTO> &>(
                *packet_list.packets[0].structDTOPtr)
                .getStructDTOClass();

        ResponceDTO responce_dto;
        responce_dto.anyNumber = 0;

        if (CheckUserPassword(session, credentials)) {
          responce_dto.reqResult = true;
          responce_dto.anyString = credentials.login;
        } else {
          responce_dto.reqResult = false;
          responce_dto.anyString = "@";
        }

        response_packet.structDTOPtr =
            std::make_shared<StructDTOClass<ResponceDTO>>(responce_dto);
        response_packet_list.packets.push_back(response_packet);
        break;
      }
      case RequestType::RqFrClientRegisterUser: {
        if (packet_list.packets.empty()) {
          throw exc::EmptyPacketException();
        }

        const auto &login_packet =
            static_cast<const StructDTOClass<UserLoginDTO> &>(
                *packet_list.packets[0].structDTOPtr)
                .getStructDTOClass();

        const auto device_payload =
            session.registerOnDeviceDataSrvSQL(login_packet.login);
        if (!device_payload.has_value()) {
          return false;
        }

        response_packet_list = device_payload.value();
        break;
      }
      case RequestType::RqFrClientFindUserByPart: {
        if (packet_list.packets.empty()) {
          throw exc::EmptyPacketException();
        }

        const auto &login_packet =
            static_cast<const StructDTOClass<UserLoginPasswordDTO> &>(
                *packet_list.packets[0].structDTOPtr)
                .getStructDTOClass();

        const auto &user_vector =
            sql_requests_.getUsersByTextPartSQL(session.getPGConnection(),
                                                login_packet);

        if (user_vector.has_value() && !user_vector->empty()) {
          PacketDTO packet;
          packet.requestType = request_type;
          packet.structDTOClassType = StructDTOClassType::userDTO;
          packet.reqDirection = RequestDirection::ClientToSrv;

          for (const auto &user_dto : user_vector.value()) {
            packet.structDTOPtr =
                std::make_shared<StructDTOClass<UserDTO>>(user_dto);
            response_packet_list.packets.push_back(packet);
          }
        } else {
          ResponceDTO responce_dto{};
          responce_dto.reqResult = false;
          responce_dto.anyNumber = 0;
          responce_dto.anyString = "";

          PacketDTO packet;
          packet.requestType = request_type;
          packet.structDTOClassType = StructDTOClassType::responceDTO;
          packet.reqDirection = RequestDirection::ClientToSrv;
          packet.structDTOPtr =
              std::make_shared<StructDTOClass<ResponceDTO>>(responce_dto);
          response_packet_list.packets.push_back(packet);
        }
        break;
      }
      case RequestType::RqFrClientSetLastReadMessage: {
        if (packet_list.packets.empty()) {
          throw exc::EmptyPacketException();
        }

        PacketDTO response_packet;
        response_packet.requestType = request_type;
        response_packet.structDTOClassType = StructDTOClassType::responceDTO;
        response_packet.reqDirection = RequestDirection::ClientToSrv;

        const auto &message_packet =
            static_cast<const StructDTOClass<MessageDTO> &>(
                *packet_list.packets[0].structDTOPtr)
                .getStructDTOClass();

        auto value = sql_requests_.setLastReadMessageSQL(
            session.getPGConnection(), message_packet);

        ResponceDTO responce_dto;
        responce_dto.anyNumber = 0;
        responce_dto.anyString = "";
        responce_dto.reqResult = value;

        if (!value) {
          std::cerr << "Сервер: RqFrClientSetLastReadMessage."
                    << " Ошибка базы."
                    << " Значение не установлено."
                    << std::endl;
        }

        response_packet.structDTOPtr =
            std::make_shared<StructDTOClass<ResponceDTO>>(responce_dto);
        response_packet_list.packets.push_back(response_packet);
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

bool UserRegistrationProcessor::CheckUserLogin(ServerSession &session,
                                               const std::string &login) {
  PGresult *result = nullptr;

  std::string sql;
  std::string login_escaped = login;

  try {
    for (std::size_t pos = 0;
         (pos = login_escaped.find('\'', pos)) != std::string::npos; pos += 2) {
      login_escaped.replace(pos, 1, "''");
    }

    sql =
        "select id from public.users as u where u.login = '" + login_escaped +
        "';";

    result = sql_requests_.execSQL(session.getPGConnection(), sql);

    if (result == nullptr) {
      throw exc::SQLSelectException(
          ", UserRegistrationProcessor::CheckUserLogin");
    }

    if (PQresultStatus(result) == PGRES_TUPLES_OK && PQntuples(result) > 0) {
      PQclear(result);
      return true;
    }

    PQclear(result);
    return false;
  } catch (const exc::SQLSelectException &ex) {
    std::cerr << "Сервер: " << ex.what() << std::endl;
    if (result != nullptr) {
      PQclear(result);
    }
    return false;
  }
}

bool UserRegistrationProcessor::CheckUserPassword(
    ServerSession &session, const UserLoginPasswordDTO &credentials) {
  PGresult *result = nullptr;

  std::string sql;
  std::string login_escaped = credentials.login;
  std::string password_escaped = credentials.passwordhash;

  try {
    for (std::size_t pos = 0;
         (pos = login_escaped.find('\'', pos)) != std::string::npos; pos += 2) {
      login_escaped.replace(pos, 1, "''");
    }

    for (std::size_t pos = 0; (pos = password_escaped.find('\'', pos)) !=
                                 std::string::npos; pos += 2) {
      password_escaped.replace(pos, 1, "''");
    }

    sql =
        "with user_record as (\n"
        "    select id as user_id\n"
        "    from public.users\n"
        "    where login = '" +
        login_escaped +
        "')\n"
        "select password_hash from public.users_passhash as ph join "
        "user_record ur on ph.user_id = ur.user_id where password_hash = '" +
        password_escaped + "';";

    result = sql_requests_.execSQL(session.getPGConnection(), sql);

    if (result == nullptr) {
      throw exc::SQLSelectException(
          ", UserRegistrationProcessor::CheckUserPassword");
    }

    if (PQresultStatus(result) == PGRES_TUPLES_OK && PQntuples(result) > 0) {
      PQclear(result);
      return true;
    }

    PQclear(result);
    return false;
  } catch (const exc::SQLSelectException &ex) {
    std::cerr << "Сервер: " << ex.what() << std::endl;
    if (result != nullptr) {
      PQclear(result);
    }
    return false;
  }
}
