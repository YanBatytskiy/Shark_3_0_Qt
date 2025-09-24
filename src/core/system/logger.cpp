#include "logger.h"
#include <cstddef>

Logger::Logger() {
  _writer.open(_logFileName, std::ios::out | std::ios::app);
  _reader.open(_logFileName, std::ios::in);
}

Logger::~Logger() {
  _writer.close();
  _reader.close();
}

bool Logger::writeLine(const std::string &str) {

  if (_writer.is_open()) {

    _mutex.lock();
    _writer.seekp(0, std::ios::end);
    _writer << str << std::endl;
    _mutex.unlock();

    return true;
  } // if open
  return false;
}

std::string Logger::readLine(const std::size_t lineIndex) {

  std::string result;

  if (_reader.is_open()) {
    _mutex.lock_shared();

    _reader.seekp(0, std::ios::end);
    _reader >> result;

    _mutex.unlock_shared();
    return result;
  }
  return "-1";
}