#include "logger.h"
#include <QApplication>
#include <QThread>
#include <QVector>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <vector>

Logger::Logger() {

  QString path = QCoreApplication::applicationDirPath() + "/log.txt";
  _logFileName = path.toStdString();

  _writer.open(_logFileName, std::ios::out | std::ios::app);
  _reader.open(_logFileName, std::ios::in);

  this->moveToThread(&loggerThread);
  loggerThread.start();
}

Logger::~Logger() {
  loggerThread.quit();
  loggerThread.wait();

  _writer.flush();
  _writer.close();
  _reader.close();
}

bool Logger::slotWriteLine(const QString &logLine) {

  if (!_writer.is_open())
    return false;

  std::unique_lock<std::shared_mutex> lk(_mutex);

  _writer.seekp(0, std::ios::end);
  _writer << logLine.toStdString() << '\n';
  _writer.flush();

  return true;
}

QVector<QString> Logger::slotReadLastLine() {

  QVector<QString> result;
  result.clear();

  if (!_reader.is_open())
    return result;

  std::shared_lock<std::shared_mutex> lk(_mutex);

  _reader.clear();
  _reader.seekg(0, std::ios::end);
  std::streampos pos = _reader.tellg();

  if (pos == std::streampos(0))
    return result;

  // шаг назад от конца
  pos = -std::streamoff(1);

  // если в конце стоит '\n', пропускаем хвостовые переводы строки
  char ch = '\0';
  _reader.seekg(pos);
  _reader.get(ch);

  while (pos > std::streampos(0) && ch == '\n') {
    pos -= std::streamoff(1);
    _reader.seekg(pos);
    _reader.get(ch);
  }

  // идём назад до начала строки или начала файла
  while (pos > std::streampos(0) && ch != '\n') {
    pos -= std::streamoff(1);
    _reader.seekg(pos);
    _reader.get(ch);
  }

  // если нашли '\n', сместиться на символ вперёд к началу строки
  if (ch == '\n')
    pos += std::streamoff(1);

  std::string str;
  _reader.clear();
  _reader.seekg(pos);
  std::getline(_reader, str);
  if (str.empty())
    return result;

  result.push_back(QString::fromStdString(str));

  return result;
}

QVector<QString> Logger::slotReadSeveralLines(qint64 linesToRead) {

  QVector<QString> result;
  result.clear();

  if (!_reader.is_open() || linesToRead <= 0)
    return result;

  const auto quantityLines = slotgetLineCount();
  if (quantityLines <= 0)
    return result;

  std::shared_lock<std::shared_mutex> lk(_mutex);

  _reader.clear();
  _reader.seekg(0, std::ios::end);
  std::streampos pos = _reader.tellg();
  if (pos == std::streampos(0))
    return result; // пустой файл

  // начать с последнего байта
  pos -= std::streamoff(1);

  // пропустить хвостовые '\n'
  char ch = '\0';
  _reader.seekg(pos);
  _reader.get(ch);

  while (pos > std::streampos(0) && ch == '\n') {
    pos -= std::streamoff(1);
    _reader.seekg(pos);
    _reader.get(ch);
  }

  // собрать последние N строк, двигаясь назад
  std::vector<std::string> lines; // временно в прямом виде
  while (true) {
    // дойти до начала строки или файла
    while (pos > std::streampos(0) && ch != '\n') {
      pos -= std::streamoff(1);
      _reader.seekg(pos);
      _reader.get(ch);
    }

    // позиция стоит на '\n' или на 0
    std::streampos lineStart;
    if (ch == '\n') {
      lineStart = pos + std::streamoff(1);
    } else {
      lineStart = std::streampos(0);
    }

    // прочитать строку
    _reader.clear();
    _reader.seekg(lineStart);
    std::string s;
    std::getline(_reader, s);
    lines.push_back(std::move(s));

    if (static_cast<qint64>(lines.size()) >= linesToRead)
      break;
    if (lineStart == std::streampos(0))
      break; // достигли начала файла

    // перейти перед найденную строку и продолжить поиск предыдущей
    pos = lineStart - std::streamoff(1);
    _reader.seekg(pos);
    _reader.get(ch);
  }

  // вернуть в правильном порядке от старой к новой
  for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
    result.push_back(QString::fromStdString(*it));
  }
  return result;
}

qint64 Logger::slotgetLineCount() {
  if (!_reader.is_open())
    return -1;

  std::shared_lock<std::shared_mutex> lk(_mutex);

  _reader.clear();
  _reader.seekg(0, std::ios::beg);
  return std::count(std::istreambuf_iterator<char>(_reader),
                    std::istreambuf_iterator<char>(), '\n');
}

bool Logger::slotClearLogFile() {
  if (_logFileName.empty())
    return false;

  std::unique_lock<std::shared_mutex> lk(_mutex);

  _writer.flush();
  _writer.close();
  _reader.close();

  std::ofstream trunc(_logFileName, std::ios::out | std::ios::trunc);
  const bool ok = trunc.is_open();
  trunc.close();

  _writer.open(_logFileName, std::ios::out | std::ios::app);
  _reader.open(_logFileName, std::ios::in);

  return ok && _writer.is_open() && _reader.is_open();
}

void Logger::slotStopLogger() {
  loggerThread.quit();
  loggerThread.wait();
}
