#pragma once

#include "client_session.h"

class QtSession {
private:
  ClientSession _qtClientSession;

public:
  // constructor
explicit QtSession(ChatSystem &qtClientSession);

  // getters
  ClientSession &getQtClientSession();

  // setters

  // utilities
  bool checkLoginPsswordQt(std::string login, std::string password);

  bool registerClientOnDeviceQt(std::string login);

  bool inputNewLoginValidationQt(std::string inputData, std::size_t dataLengthMin, std::size_t dataLengthMax);

  bool inputNewPasswordValidationQt(std::string inputData, std::size_t dataLengthMin, std::size_t dataLengthMax);
};