#include "screen_user_data.h"
#include "ui_screen_user_data.h"
#include "model_user_list.h"
#include "system/date_time_utils.h"

ScreenUserData::ScreenUserData(QWidget *parent) : QWidget(parent), ui(new Ui::ScreenUserData) { ui->setupUi(this);
  }

ScreenUserData::~ScreenUserData() { delete ui; }

void ScreenUserData::setDatabase(std::shared_ptr<ClientSession> sessionPtr)
{
  _sessionPtr = sessionPtr;

}

void ScreenUserData::slotClearUserDataToLabels() {

  ui->loginLineEdit->setText("");
  ui->nameLineEdit->setText("");
  ui->emailLineEdit->setText("");
  ui->phoneLineEdit->setText("");

  ui->blockedUserLabel->setText("");
  ui->reasonDisableabel->setText("");

  ui->unblockPushButton->setEnabled(false);
  ui->banPushButton->setEnabled(false);
  ui->blockPushButton->setEnabled(false);
}

void ScreenUserData::setUserDataToLabels(const QModelIndex &index)
{

  QString textValue = index.data(UserListModel::LoginRole).toString();
  ui->loginLineEdit->setText(textValue);

  textValue = index.data(UserListModel::NameRole).toString();
  ui->nameLineEdit->setText(textValue);

  textValue = index.data(UserListModel::EmailRole).toString();
  ui->emailLineEdit->setText(textValue);

  textValue = index.data(UserListModel::PhoneRole).toString();
  ui->phoneLineEdit->setText(textValue);

  const auto &isActive = index.data(UserListModel::IsActiveRole).toBool();
  const auto& reasonDisable = index.data(UserListModel::DisableReasonRole).toString();

  const auto &bunTo = index.data(UserListModel::BunUntilRole).toLongLong();

  if (bunTo != 0 && bunTo > getCurrentDateTimeInt()) {
    ui->bunnedToDateUserLabel->setText("Бан до: " + QString::fromStdString(
                                                        formatTimeStampToString(bunTo, true)));
    ui->unBunPushButton->setEnabled(true);
    ui->banPushButton->setEnabled(false);
    ui->reasonDisableabel->setText("Причина блокировки: " + reasonDisable);
  } else {
    ui->bunnedToDateUserLabel->setText("");
    ui->unBunPushButton->setEnabled(false);
    ui->banPushButton->setEnabled(true);
  }

  if (isActive) {
    ui->blockedUserLabel->setText("");
    ui->unblockPushButton->setEnabled(false);
    ui->blockPushButton->setEnabled(true);
    ui->reasonDisableabel->setText("");
  } else {
    ui->blockedUserLabel->setText("Пользователь заблокирован.");
    ui->unblockPushButton->setEnabled(true);
    ui->blockPushButton->setEnabled(false);
    ui->banPushButton->setEnabled(false);
    ui->reasonDisableabel->setText("Причина блокировки: " + reasonDisable);
    ui->reasonDisableabel->setText("");
  }
}
