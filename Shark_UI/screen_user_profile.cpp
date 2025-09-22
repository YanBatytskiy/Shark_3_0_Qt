#include "screen_user_profile.h"
#include "System/picosha2.h"
#include "dto/dto_struct.h"
#include "ui_screen_user_profile.h"
#include <QMessageBox>

ScreenUserProfile::ScreenUserProfile(QWidget *parent) : QWidget(parent), ui(new Ui::ScreenUserProfile) {
  ui->setupUi(this);
  _userData._name = "";
  _userData._email = "";
  _userData._phone = "";
}

ScreenUserProfile::~ScreenUserProfile() { delete ui; }

void ScreenUserProfile::setDatabase(std::shared_ptr<ClientSession> sessionPtr) {
  _sessionPtr = sessionPtr;
}

const UserDataQt ScreenUserProfile::getUserData() const {
  return _userData;
}

void ScreenUserProfile::fillDataToForm(const QString &name, const QString &email, const QString &phone) {
  _userData._name = name;
  _userData._email = email;
  _userData._phone = phone;
}

void ScreenUserProfile::clearDataOnForm() {
  _userData._name = "";
  _userData._email = "";
  _userData._phone = "";

  _isNameChanged = false;
  _isEmailChanged = false;
  _isPhoneChanged = false;
  _isPasswordChanged = false;

  ui->passwordLineEdit->clear();
  ui->confirnPasswordLineEdit->clear();

  ui->savePushButton->setEnabled(false);
  ui->changePasswordPushButton->setEnabled(false);
}

void ScreenUserProfile::on_cancelPushButton_clicked()
{

  clearDataOnForm();

  emit signalCloseUserProfile();
}


void ScreenUserProfile::on_savePushButton_clicked()
{
  if (!_isNameChanged || !_isEmailChanged || !_isPhoneChanged)
    return;

  UserDTO userDTO;
  userDTO.login = ui->loginLineEdit->text().toStdString();
  userDTO.userName = ui->nameLineEdit->text().toStdString();
  userDTO.email = ui->emailLineEdit->text().toStdString();
  userDTO.phone = ui->phoneLineEdit->text().toStdString();

  _isNameChanged = false;
  _isEmailChanged = false;
  _isPhoneChanged = false;
  _isPasswordChanged = false;

  if (_sessionPtr->changeUserDataQt(userDTO))
    QMessageBox::information(this, "Сообщение", "Данные изменены.");
  else
    QMessageBox::critical(this, "Ошибка", "Данные не изменены.");
}

void ScreenUserProfile::on_nameLineEdit_editingFinished() {

  if (ui->nameLineEdit->text() == _userData._name) {
    _isNameChanged = false;
    return;
  }

  if (ui->nameLineEdit->text().size() < 2 || ui->nameLineEdit->text().size() > 20) {
    QMessageBox::critical(this, "Ошибка", "Некорректная длина имени. Длина должна быть от 2 до 20 символов.");
    ui->nameLineEdit->setStyleSheet("QLineEdit { color: red; }");
    _isNameChanged = false;
    ui->savePushButton->setEnabled(false);
    return;
  }

  if (!_sessionPtr->inputNewLoginValidationQt(ui->nameLineEdit->text().toStdString())) {
    QMessageBox::critical(this, "Ошибка", "Недопустимое имя. Наведи мышку на поле для полсказки.");
    ui->nameLineEdit->setStyleSheet("QLineEdit { color: red; }");
    _isNameChanged = false;
    ui->savePushButton->setEnabled(false);
  } else {
    ui->nameLineEdit->setStyleSheet("QLineEdit { color: black; }");
    ui->savePushButton->setEnabled(true);
    _isNameChanged = true;
  }
}

void ScreenUserProfile::on_emailLineEdit_editingFinished() {
  if (ui->emailLineEdit->text() == _userData._email) {
    _isEmailChanged = false;
    return;
  } else {
    _isEmailChanged = true;
    ui->savePushButton->setEnabled(true);
  }
}

void ScreenUserProfile::on_phoneLineEdit_editingFinished() {

  if (ui->phoneLineEdit->text() == _userData._phone) {
    _isPhoneChanged = false;
    return;
  } else {
    _isPhoneChanged = true;
    ui->savePushButton->setEnabled(true);
  }
}

void ScreenUserProfile::on_passwordLineEdit_editingFinished() {

  if (ui->passwordLineEdit->text().size() < 5 || ui->passwordLineEdit->text().size() > 20) {
    QMessageBox::critical(this, "Ошибка", "Некорректная длина пароля. Длина должна быть от 5 до 20 символов.");
    ui->passwordLineEdit->setStyleSheet("QLineEdit { color: red; }");
    _isPasswordChanged = false;
    return;
  }

  if (!_sessionPtr->inputNewPasswordValidationQt(ui->passwordLineEdit->text().toStdString(), 5, 20)) {
    QMessageBox::critical(this, "Ошибка", "Недопустимый пароль. Наведи мышку на поле для полсказки.");
    ui->passwordLineEdit->setStyleSheet("QLineEdit { color: red; }");
  } else
    ui->passwordLineEdit->setStyleSheet("QLineEdit { color: black; }");

  _isPasswordChanged = false;
  ui->confirnPasswordLineEdit->text() = "";
}

void ScreenUserProfile::on_confirnPasswordLineEdit_editingFinished() {
  if (ui->confirnPasswordLineEdit->text() != ui->confirnPasswordLineEdit->text()) {
    QMessageBox::critical(this, "Ошибка", "Пароли не совпадают.");
    ui->confirnPasswordLineEdit->setStyleSheet("QLineEdit { color: red; }");
    ui->changePasswordPushButton->setEnabled(false);
    _isPasswordChanged = false;
  } else {
    ui->confirnPasswordLineEdit->setStyleSheet("QLineEdit { color: black; }");
    ui->changePasswordPushButton->setEnabled(true);
    _isPasswordChanged = true;
  }
}

void ScreenUserProfile::on_changePasswordPushButton_clicked() {

  UserDTO userDTO;
  userDTO.login = ui->loginLineEdit->text().toStdString();
  userDTO.passwordhash = picosha2::hash256_hex_string(ui->passwordLineEdit->text().toStdString());

  if (_sessionPtr->changeUserPasswordQt(userDTO))
    QMessageBox::information(this, "Сообщение", "Пароль изменен.");
  else
    QMessageBox::critical(this, "Ошибка", "Пароль не изменен.");
}
