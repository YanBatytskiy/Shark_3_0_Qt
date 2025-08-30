#pragma once

#include "client_session.h"

class Qt_Session {
private:
  ClientSession _qtClientSession;

public:
  // constructor
  Qt_Session(ClientSession &qtClientSession);

  // getters
  ClientSession& getQtClientSession();

  // setters

  // utilities
  bool checkLoginPsswordQt(std::string login, std::string password);

  bool registerClientOnDeviceQt(std::string login);
      
};