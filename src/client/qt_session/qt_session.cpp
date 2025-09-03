#include "qt_session.h"
#include "chat_system/chat_system.h"
#include "client_session.h"
#include "system/picosha2.h"
#include "system/system_function.h"

// constructors
QtSession::QtSession(ChatSystem& qtClientSession) : _qtClientSession(qtClientSession) {}

// getters
ClientSession &QtSession::getQtClientSession() { return _qtClientSession;; }

// setters

// itilities
bool QtSession::checkLoginPsswordQt(std::string login, std::string password) {

  auto passHash = picosha2::hash256_hex_string(password);
 
  return _qtClientSession.checkUserPasswordCl(login, passHash);
}

bool QtSession::registerClientOnDeviceQt(std::string login) { return _qtClientSession.registerClientToSystemCl(login); }

bool QtSession::inputNewLoginValidationQt(std::string inputData, std::size_t dataLengthMin, std::size_t dataLengthMax) {

  // проверяем только на англ буквы и цифры
  if (!engAndFiguresCheck(inputData))
    return false;
  else
    return true;
}

bool QtSession::inputNewPasswordValidationQt(std::string inputData, std::size_t dataLengthMin,
                                             std::size_t dataLengthMax) {

  // проверяем только на англ буквы и цифры
  if (!engAndFiguresCheck(inputData))
    return false;

  if (!checkNewLoginPasswordForLimits(inputData, dataLengthMin, dataLengthMax, true))
    return false;
  else
    return true;
}
