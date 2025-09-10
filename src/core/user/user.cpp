#include "user.h"
#include "exceptions_cpp/validation_exception.h"
#include "system/date_time_utils.h"
#include "user_chat_list.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>

User::User(const UserData &userData) : _userData(userData) {}

/**
 * @brief Assigns a chat list to the user.
 * @param userChats shared pointer to the user's chat list.
 */
void User::createChatList() { _userChatList = std::make_shared<UserChatList>(shared_from_this()); }

void User::createChatList(const std::shared_ptr<UserChatList> &userChatList) { _userChatList = userChatList; }

std::string User::getLogin() const { return _userData._login; }

std::string User::getUserName() const { return _userData._userName; }

std::string User::getPasswordHash() const { return _userData._passwordHash; }

std::string User::getEmail() const { return _userData._email; }

std::string User::getPhone() const { return _userData._phone; }

std::string User::getDisableReason() const { return _userData._disableReason; }

bool User::getIsActive() const { return _userData._isActive; }

std::int64_t User::getDisabledAt() const { return _userData._disabledAt; }

std::int64_t User::getBunUntil() const { return _userData._bunUntil; }

std::shared_ptr<UserChatList> User::getUserChatList() const { return _userChatList; }

void User::setLogin(const std::string &login) { _userData._login = login; }

void User::setUserName(const std::string &userName) { _userData._userName = userName; }

void User::setPassword(const std::string &passwordHash) { _userData._passwordHash = passwordHash; }

void User::setEmail(const std::string &email) { _userData._email = email; }

void User::setPhone(const std::string &phone) { _userData._phone = phone; }

bool User::checkPassword(const std::string &passwordHash) const { return (passwordHash == getPasswordHash()); }

bool User::RqFrClientCheckLogin(const std::string &login) const { return (getLogin() == login); }

void User::showUserData() const { std::cout << "Name: " << getUserName() << ", Login: " << getLogin() << std::endl; }

void User::showUserDataInit() const { std::cout << "Name: " << getUserName() << ", Login: " << getLogin(); }

void User::printChatList(const std::shared_ptr<User> &user) const {
  // ДОДЕЛАТЬ ВЫВОД УДАЛЕННОГО ПОЛЬЗОВАТЕЛЯ В СПИСКЕ а также количество новых
  // сообщений в списке

  // достаем чатлист
  const auto &chatList = user->getUserChatList()->getChatFromList();

  if (chatList.empty()) {
    std::cout << "У пользователя " << user->getUserName() << " нет чатов." << std::endl;
    return;
  }
  std::cout << std::endl
            << "Всего чатов = " << user->getUserChatList()->getChatFromList().size() << ". Список чатов пользователя "
            << user->getUserName() << " :" << std::endl;

  std::size_t index = 1;

  // перебираем чаты в списке
  for (const auto &weakChat : chatList) {

    if (auto chat_ptr = weakChat.lock()) {

      std::cout << std::endl;
      std::cout << index << ". chatId: " << chat_ptr->getChatId() << ", Всего сообщений: ";

      const auto &messages = chat_ptr->getMessages();
      try {
        if (messages.empty())
          throw exc::MessagesNotFoundException();
        else
          std::cout << messages.size() << ", ";

      } catch (const exc::ValidationException &ex) {
        std::cout << " ! " << ex.what() << std::endl;
      }

      // перебираем участников чата
      std::cout << "Имя/Логин: ";
      for (const auto &participant : chat_ptr->getParticipants()) {
        auto user_ptr = participant._user.lock();

        if (user_ptr) {
          if (user_ptr != user) {
            std::cout << user_ptr->getUserName() << "/" << user_ptr->getLogin() << "; ";
          }
        } // if (user_ptr)
        else {
          std::cout << " удал. пользователь., ";
        }
      } // for participant
      // выводим на печать дату и время последнего сообщения
      const auto &lastMessage_ptr = std::prev(messages.end())->second;
      const auto &timeStampLastMessage = lastMessage_ptr->getTimeStamp();

      std::cout << "Последнее сообщение - " << formatTimeStampToString(timeStampLastMessage, true) << ". ";

      // вывод на печать количества новых сообщений

      std::cout << "новых сообщений - " << chat_ptr->getUnreadMessageCount(user) << std::endl;
    } // if auto chat_ptr
    else {
      std::cout << "Чат удален." << std::endl;
    }
    ++index;

  } // for (const auto &weakChat
}