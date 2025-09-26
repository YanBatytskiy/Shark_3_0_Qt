#include "exceptions_qt/errorbus.h"

#include "logger.h"
#include "screen_login.h"
#include "ui_screen_login.h"

#include <sys/utsname.h>

#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

ScreenLogin::ScreenLogin(QWidget *parent) : QWidget(parent), ui(new Ui::ScreenLogin) {
  ui->setupUi(this);

  connect(ui->logFileClearPushButton, &QPushButton::clicked,
          this, &ScreenLogin::slotOn_logFileClearPushButton_clicked);

  connect(ui->lookLogSeveralLinePushButton, &QPushButton::clicked,
          this, &ScreenLogin::slotOn_lookLogSeveralLinePushButton_clicked);

  connect(ui->lookLogLastLinePushButton, &QPushButton::clicked,
          this, &ScreenLogin::slotOn_lookLogLastLinePushButton_clicked);
}

ScreenLogin::~ScreenLogin() { delete ui; }

void ScreenLogin::setDatabase(std::shared_ptr<ClientSession> sessionPtr, std::shared_ptr<Logger> loggerPtr) {

  _sessionPtr = sessionPtr;
  _loggerPtr = loggerPtr;

  connect(_sessionPtr.get(), &ClientSession::serverStatusChanged, this, &ScreenLogin::onConnectionStatusChanged,
          Qt::QueuedConnection);

  struct utsname utsname;
  uname(&utsname);

  _systemData = "OS name: " + QString::fromStdString(utsname.sysname) +
                "\nOS Release: " + QString::fromStdString(utsname.release) +
                "\nOS version: " + QString::fromStdString(utsname.version) +
                "\nArchitecture: " + QString::fromStdString(utsname.machine) +
                "\nHost name: " + QString::fromStdString(utsname.nodename);

  QString serverDataText;

  serverDataText = _systemData;

  auto mode = _sessionPtr->getserverConnectionModeCl();

  if (mode == ServerConnectionMode::Localhost)
    serverDataText += "\n\nПодключено к серверу внутри рабочей станции.";
  else if (mode == ServerConnectionMode::LocalNetwork)
    serverDataText += "\n\nПодключено к серверу внути локальной сети.";
  else if (mode == ServerConnectionMode::Offline)
    serverDataText += "\n\nПодключение к серверу не удалось. Режим Offline.";

  ServerConnectionConfig serverConnectionConfig;

  if (mode == ServerConnectionMode::Localhost) {
    serverDataText += QString::fromStdString("\nLocalHost address: ");
    serverDataText += QString::fromStdString(serverConnectionConfig.addressLocalHost);
    serverDataText += QString::fromStdString("\nPort: ");
    serverDataText += QString::number(serverConnectionConfig.port);
  } else if (mode == ServerConnectionMode::LocalNetwork) {
    serverDataText += QString::fromStdString("\nLocal Network Address: ");
    serverDataText += QString::fromStdString(serverConnectionConfig.addressLocalNetwork);
    serverDataText += QString::fromStdString("\nPort: ");
    serverDataText += QString::number(serverConnectionConfig.port);
  };

  ui->serverDataLabel->setText(serverDataText);
}

void ScreenLogin::clearFields() {
  ui->loginEdit->clear();
  ui->passwordEdit->clear();
  ui->loginEdit->setFocus();
}

void ScreenLogin::onConnectionStatusChanged(bool connectionStatus, ServerConnectionMode mode) {

  QString serverDataText;

  serverDataText = _systemData;

  if (mode == ServerConnectionMode::Localhost)
    serverDataText += "\n\nПодключено к серверу внутри рабочей станции.";
  else if (mode == ServerConnectionMode::LocalNetwork)
    serverDataText += "\n\nПодключено к серверу внути локальной сети.";
  else if (mode == ServerConnectionMode::Offline)
    serverDataText += "\n\nПодключение к серверу не удалось. Режим Offline.";

  ServerConnectionConfig serverConnectionConfig;

  if (mode == ServerConnectionMode::Localhost) {
    serverDataText += QString::fromStdString("\nLocalHost address: ");
    serverDataText += QString::fromStdString(serverConnectionConfig.addressLocalHost);
    serverDataText += QString::fromStdString("\nPort: ");
    serverDataText += QString::number(serverConnectionConfig.port);
  } else if (mode == ServerConnectionMode::LocalNetwork) {
    serverDataText += QString::fromStdString("\nLocal Network Address: ");
    serverDataText += QString::fromStdString(serverConnectionConfig.addressLocalNetwork);
    serverDataText += QString::fromStdString("\nPort: ");
    serverDataText += QString::number(serverConnectionConfig.port);
  };

  ui->serverDataLabel->setText(serverDataText);

  if (connectionStatus) {
    ui->serverStatusLabelRound->setStyleSheet("background-color: green; border-radius: 8px;");
    ui->serverStatusLabel->setText("server online");
  } else {
    ui->serverStatusLabelRound->setStyleSheet("background-color: red; border-radius: 8px;");
    ui->serverStatusLabel->setText("server offline");
  }
}

void ScreenLogin::slot_show_logger_form(const std::multimap<qint64, QString> &logger_model) {
  if (logger_form_ == nullptr)
    logger_form_ = new ScreenLoggerForm(_sessionPtr, _loggerPtr, this);

  logger_form_->slot_fill_logger_model(logger_model);

  logger_form_->setWindowModality(Qt::ApplicationModal);
  logger_form_->setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint);
  logger_form_->show();
}

void ScreenLogin::on_registerModeButton_clicked() {

  if (!_sessionPtr->getIsServerOnline()) {
    QMessageBox::warning(this, tr("No!"), tr("Сервер не доступен."));
  } else {
    clearFields();
    emit registrationRequested();
  }
}

void ScreenLogin::on_loginButtonBox_accepted() { checkSignIn(); }

void ScreenLogin::on_loginButtonBox_rejected() { emit rejected(); }

void ScreenLogin::checkSignIn() {

  try {

    const auto newInput = ui->passwordEdit->text().toStdString();

    if (ui->loginEdit->text().toStdString() == "" || newInput == "")
      return;

    if (!_sessionPtr->getIsServerOnline()) {
      QMessageBox::warning(this, tr("No!"), tr("Сервер не доступен."));
      return;
    }

    auto result = _sessionPtr->checkLoginPsswordQt(ui->loginEdit->text().toStdString(),
                                                   ui->passwordEdit->text().toStdString());

    if (!result) {
      emit exc_qt::ErrorBus::i().error(tr("Login or Password is wrong"), "login");
      return;
    }

    emit accepted(ui->loginEdit->text());

  } catch (const std::exception &) {
    return;
  }
}

void ScreenLogin::on_loginEdit_returnPressed() {
  if (ui->loginEdit->text().toStdString() != "")
    ui->passwordEdit->setFocus();
}

void ScreenLogin::on_passwordEdit_returnPressed() {
  if (ui->loginEdit->text().toStdString() == "" || ui->passwordEdit->text().toStdString() == "")
    return;
  checkSignIn();
}

void ScreenLogin::on_baseReInitialisationPushButton_clicked() {

  bool result = _sessionPtr->reInitilizeBaseQt();

  if (!result)
    QMessageBox::warning(this, "Ошибка", "Не удалось переинициализировать базу.");
  else {
    QMessageBox::information(this, "Успешно.", "Можно входить.");
  }
}

void ScreenLogin::slotOn_logFileClearPushButton_clicked() {

  QMessageBox message(this);
  message.setWindowTitle("Подтверждение.");
  message.setText("Вы уверены, что хотите очистить файл логов?");
  auto clearButton = message.addButton("Очистить", QMessageBox::AcceptRole);
  message.addButton("Отмена", QMessageBox::RejectRole);

  message.exec();

  const auto clicked = message.clickedButton();
  if (clicked != clearButton)
    return;

  if (!_loggerPtr) {
    QMessageBox::warning(this, "Ошибка", "Логгер недоступен.");
    return;
  }

  const bool cleared = _loggerPtr->slotClearLogFile();
  if (cleared)
    QMessageBox::information(this, "Сообщение", "Файл логов очищен.");
  else
    QMessageBox::warning(this, "Ошибка", "Ошибка доступа к файлу.");
}

void ScreenLogin::slotOn_lookLogSeveralLinePushButton_clicked() {

  QMessageBox message(this);
  message.setWindowTitle("Выберите сколько строк выводить.");
  message.setText("Выберите некоторое количиство последних строк либо весь лог");
  message.addButton("Ввести количество строк", QMessageBox::AcceptRole);
  message.addButton("Вывести все строки", QMessageBox::AcceptRole);
  message.addButton("Отмена", QMessageBox::RejectRole);

  message.exec();

  if (message.clickedButton()->text() == "Ввести количество строк") {
    bool ok;
    int count = QInputDialog::getInt(this, "Количество:", "Ведите количество:", 10, 1, 1000, 1, &ok);
    if (ok) {
      const auto &model_data = _loggerPtr->slotReadSeveralLines(count);
      slot_show_logger_form(model_data);
    }
  } else if (message.clickedButton()->text() == "Вывести все строки") {
    const auto &model_data = _loggerPtr->slotReadSeveralLines(0);
    slot_show_logger_form(model_data);
  }
}

void ScreenLogin::slotOn_lookLogLastLinePushButton_clicked() {
  const auto &model_data = _loggerPtr->slotReadLastLine();

  slot_show_logger_form(model_data);
}
