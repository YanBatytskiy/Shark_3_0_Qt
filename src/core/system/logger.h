#pragma once
#include <fstream>
#include <shared_mutex>
#include <string>
;

class Logger {
private:
  std::shared_mutex _mutex;
  std::fstream _writer;
  std::fstream _reader;
  std::string _logFileName{"log.txt"};

public:
  Logger();
  ~Logger();

  bool writeLine(const std::string& str);
  std::string readLine(const std::size_t lineIndex);
  std::string readNextLine();
  std::size_t getLineCount();

  void clear();

};