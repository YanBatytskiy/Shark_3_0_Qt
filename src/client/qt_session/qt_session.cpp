#include "qt_session.h"
#include "client_session.h"
#include "system/picosha2.h"

// constructors
Qt_Session::Qt_Session(ClientSession &qtClientSession) : _qtClientSession(qtClientSession) {}

// getters
ClientSession &Qt_Session::getQtClientSession() { return _qtClientSession; }

// setters

// itilities
bool Qt_Session::checkLoginPsswordQt(std::string login, std::string password) {

  auto passHash = picosha2::hash256_hex_string(password);

  return _qtClientSession.checkUserPasswordCl(login, passHash);
}

bool Qt_Session::registerClientOnDeviceQt(std::string login) {

  return _qtClientSession.registerClientToSystemCl(login);
}
