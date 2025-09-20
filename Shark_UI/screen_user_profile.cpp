#include "screen_user_profile.h"
#include "ui_screen_user_profile.h"

ScreenUserProfile::ScreenUserProfile(QWidget *parent) : QWidget(parent), ui(new Ui::ScreenUserProfile) {
  ui->setupUi(this);
}

ScreenUserProfile::~ScreenUserProfile() { delete ui; }

void ScreenUserProfile::on_cancelPushButton_clicked()
{
  _isDataChanged = false;
  ui->passwordLineEdit->clear();
  ui->confirnPasswordLineEdit->clear();
  ui->savePushButton->setEnabled(false);
  ui->changePasswordPushButton->setEnabled(false);
 emit signalCloseUserProfile();
}


void ScreenUserProfile::on_savePushButton_clicked()
{
  _isDataChanged = false;
}

void ScreenUserProfile::on_nameLineEdit_textChanged(const QString &arg1)
{
  _isDataChanged = true;
  ui->savePushButton->setEnabled(true);
}


void ScreenUserProfile::on_emailLineEdit_textChanged(const QString &arg1)
{
  _isDataChanged = true;
  ui->savePushButton->setEnabled(true);
}


void ScreenUserProfile::on_phoneLineEdit_textChanged(const QString &arg1)
{
  _isDataChanged = true;
  ui->savePushButton->setEnabled(true);

}


void ScreenUserProfile::on_confirnPasswordLineEdit_textChanged(const QString &arg1)
{
  if (ui->passwordLineEdit->text()!="" && ui->confirnPasswordLineEdit->text() != "") {

    if (ui->passwordLineEdit->text() == ui->confirnPasswordLineEdit->text())
      ui->changePasswordPushButton->setEnabled(true);
    else
      ui->changePasswordPushButton->setEnabled(false);
  }
  else
    ui->changePasswordPushButton->setEnabled(false);
}

