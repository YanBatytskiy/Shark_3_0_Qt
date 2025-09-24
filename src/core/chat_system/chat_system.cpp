#include "chat_system.h"
#include "chat/chat.h"
#include "dto/dto_struct.h"
#include "exceptions_cpp/login_exception.h"
#include "exceptions_cpp/validation_exception.h"
#include "system/system_function.h"
#include "user/user_chat_list.h"
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

/**
 * @brief Default constructor for ChatSystem.
 */
ChatSystem::ChatSystem() {}

// getters

const bool &ChatSystem::getIsServerStatus() const { return _isServerStatus; }

std::shared_ptr<Chat> ChatSystem::getChatById(std::size_t chatId) const {
  auto it = _chatIdChatMap.find(chatId);
  if (it != _chatIdChatMap.end())
    return it->second;
  return nullptr;
}

const std::vector<std::shared_ptr<User>> &ChatSystem::getUsers() const { return _users; }

const std::vector<std::shared_ptr<Chat>> &ChatSystem::getChats() const { return _chats; }

const std::shared_ptr<User> &ChatSystem::getActiveUser() const { return _activeUser; }

const std::unordered_map<std::string, std::shared_ptr<User>> &ChatSystem::getLoginUserMap() const {
  return _loginUserMap;
}

// setters

void ChatSystem::setIsServerStatus(const bool &serverStatus) { _isServerStatus = serverStatus; }

void ChatSystem::setActiveUser(const std::shared_ptr<User> &user) { _activeUser = user; }

void ChatSystem::addUserToSystem(std::shared_ptr<User> &user) {
  _users.push_back(user);
  _loginUserMap.insert({user->getLogin(), user});
}

bool ChatSystem::addChatToInstance(const std::shared_ptr<Chat> &chat_ptr) {

  if (!chat_ptr)
    throw exc::ChatNotFoundException();

  const std::size_t chatId = chat_ptr->getChatId();
  bool result = true;

  // чтобы сделать откат
  std::vector<std::shared_ptr<User>> addParticipantsChatList;

  try {
    if (_isServerStatus) {
      for (const auto &participant : chat_ptr->getParticipants()) {
        if (auto user_ptr = participant._user.lock()) {

          // добавление чата в чат-лист
          auto userChatList = user_ptr->getUserChatList();

          if (userChatList) {
            userChatList->addChatToChatList(chat_ptr);
            addParticipantsChatList.push_back(user_ptr);
          } else
            throw exc::ChatListNotFoundException(user_ptr->getLogin());
        } // if user_ptr
        else
          throw exc::UserNotFoundException();
      }
    } else
      _activeUser->getUserChatList()->addChatToChatList(chat_ptr);

    const auto temp = _activeUser->getUserChatList()->getChatFromList();

    int x = 0;

  } // try
  catch (const exc::ChatListNotFoundException &ex) {
    std::cout << " ! " << ex.what() << std::endl;
    result = false;
  } catch (const exc::ChatNotFoundException &ex) {
    std::cout << " ! " << ex.what() << std::endl;
    result = false;
  } catch (const exc::UserNotFoundException &ex) {
    std::cout << " ! " << ex.what() << std::endl;
    result = false;
  }
  if (result) {
    _chats.push_back(chat_ptr);
    _chatIdChatMap.insert({chatId, chat_ptr});
  } else {
    for (const auto &participant : addParticipantsChatList) {

      auto userChatList = participant->getUserChatList();

      userChatList->deleteChatFromList(chat_ptr);
    }
    addParticipantsChatList.clear();
  }
  return result;
}

// utilities
std::vector<std::shared_ptr<User>> ChatSystem::findUserByTextPart(
    const std::string &textToFind) const { // поиск пользователя

  std::vector<std::shared_ptr<User>> foundUsers;

  std::string textToFindLower = TextToLower(textToFind);

  // перебираем всех пользователей в векторе
  for (const auto &user : _users) {

    if (user == _activeUser)
      continue;

    std::string LowerLogin = TextToLower(user->getLogin());
    std::string LowerName = TextToLower(user->getUserName());

    if (LowerLogin.find(textToFindLower) != std::string::npos || LowerName.find(textToFindLower) != std::string::npos)
      foundUsers.push_back(user);
  }
  return foundUsers;
}

std::shared_ptr<User> ChatSystem::findUserByLogin(const std::string &userLogin) const {

  auto it = this->getLoginUserMap().find(userLogin);
  if (it == this->getLoginUserMap().end())
    return nullptr;
  else
    return it->second;
}

const std::shared_ptr<User> ChatSystem::RqFrClientCheckLoginExists(const std::string &login) const {

  auto it = this->_loginUserMap.find(login);

  if (it != this->_loginUserMap.end())
    return it->second;
  else
    return nullptr;
}

bool ChatSystem::checkPasswordValidForUser(const std::string &passwordHash, const std::string &userLogin) {

  auto user = findUserByLogin(userLogin);

  if (user == nullptr)
    return false;

  const auto &check = user->getPasswordHash();

  return passwordHash == user->getPasswordHash();
}