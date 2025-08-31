#include "chat_system/chat_system.h"
#include "client/client_session.h"
#include "qt_session/qt_session.h"

ChatSystem   clientSystem;
ClientSession clientSession(clientSystem);
QtSession     qtSession(clientSession);
