#include "screen_user_data.h"
#include "model_user_list.h"
#include "system/date_time_utils.h"
#include "ui_screen_user_data.h"
#include <QCalendarWidget>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QMessageBox>

ScreenUserData::ScreenUserData(QWidget *parent)
    : QWidget(parent), ui(new Ui::ScreenUserData) {
  ui->setupUi(this);

  connect(ui->blockPushButton, &QPushButton::clicked, this,
          &ScreenUserData::slot_on_block_push_button_clicked);

  connect(ui->unblockPushButton, &QPushButton::clicked, this,
          &ScreenUserData::slot_on_unblock_push_button_clicked);

  connect(ui->banPushButton, &QPushButton::clicked, this,
          &ScreenUserData::slot_on_bun_push_button_clicked);

  connect(ui->unBunPushButton, &QPushButton::clicked, this,
          &ScreenUserData::slot_on_unbun_push_button_clicked);
}

ScreenUserData::~ScreenUserData() { delete ui; }

void ScreenUserData::setDatabase(std::shared_ptr<ClientSession> client_session_ptr)
{
  client_session_ptr_ = client_session_ptr;

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
  ui->unBunPushButton->setEnabled(false);
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

  const auto w = ui->ScreenNewChatParticipantsWidget;

  const bool status = w->isEnabled();

  //простой просмотр данных пользователя
  if (!status)
  {
  if (bunTo != 0 && bunTo > getCurrentDateTimeInt()) {
    ui->bunnedToDateUserLabel->setText("Бан до: " + QString::fromStdString(
                                                        formatTimeStampToString(bunTo, true)));
    ui->banPushButton->setEnabled(false);
    ui->unBunPushButton->setEnabled(true);
    ui->reasonDisableabel->setText("");
  } else {
    ui->bunnedToDateUserLabel->setText("");
    ui->banPushButton->setEnabled(true);
    ui->unBunPushButton->setEnabled(false);
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
  //режим добавления контактов к новому чату
  else {
    ui->unBunPushButton->setEnabled(false);
    ui->banPushButton->setEnabled(false);
    ui->unblockPushButton->setEnabled(false);
    ui->blockPushButton->setEnabled(false);

    if (bunTo != 0 && bunTo > getCurrentDateTimeInt()) {
      ui->bunnedToDateUserLabel->setText("Бан до: " + QString::fromStdString(
                                                          formatTimeStampToString(bunTo, true)));
      ui->reasonDisableabel->setText("Причина блокировки: " + reasonDisable);
    } else {
      ui->bunnedToDateUserLabel->setText("");
    }

    if (isActive) {
      ui->blockedUserLabel->setText("");
      ui->reasonDisableabel->setText("");
    } else {
      ui->blockedUserLabel->setText("Пользователь заблокирован.");
      ui->reasonDisableabel->setText("Причина блокировки: " + reasonDisable);
      ui->reasonDisableabel->setText("");
    }

}

}

void ScreenUserData::slot_on_block_push_button_clicked() {

  QInputDialog dialog(this);
  dialog.setWindowTitle("Ввод причины блокировки");
  dialog.setLabelText("Причина блокировки (обязательно):");
  dialog.setTextValue("");
  dialog.setOption(QInputDialog::UsePlainTextEditForTextInput);

  bool ok = (dialog.exec() == QDialog::Accepted);
  if (!ok)
    return;

  const auto &block_reason = dialog.textValue();
  if (block_reason == "")
    return;

  client_session_ptr_->blockUnblockUserCl(ui->loginLineEdit->text().toStdString(), true,
                                  block_reason.toStdString());
}

void ScreenUserData::slot_on_unblock_push_button_clicked() {

  bool ok = (QMessageBox::question(
                 this, "Разблокировка", "Вы хотите снять бан с пользователя?",
                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);

  if (!ok)
    return;

  if (client_session_ptr_->blockUnblockUserCl(ui->loginLineEdit->text().toStdString(),
                                      false, "")) {
    QMessageBox::information(this, "Сообщение", "Пользователь разблокирован");
  } else {
    QMessageBox::warning(this, "Ошибка",
                         "Не смогли разблокировать пользователя");
  }
}

void ScreenUserData::slot_on_bun_push_button_clicked() {

  QDialog dialog(this);
  dialog.setWindowTitle(tr("Выбор даты"));

  QVBoxLayout *layout = new QVBoxLayout(&dialog);

  QLabel *label = new QLabel(tr("Выберите дату:"), &dialog);
  layout->addWidget(label);

  QCalendarWidget *calendar = new QCalendarWidget(&dialog);
  calendar->setSelectedDate(QDate::currentDate()); // Текущая дата по умолчанию
  layout->addWidget(calendar);

  QDialogButtonBox *buttonBox = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
  layout->addWidget(buttonBox);

  connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

  bool ok = (dialog.exec() == QDialog::Accepted);
  if (!ok)
    return;

  QDate selected_date = calendar->selectedDate();
  if (selected_date == QDate::currentDate())
    return;

  int year = selected_date.year();
  int month = selected_date.month();
  int day = selected_date.day();

  const auto time_stamp = makeTimeStamp(year, month, day, 23, 59, 59);

  client_session_ptr_->bunUnbunUserCl(ui->loginLineEdit->text().toStdString(), true,
                              time_stamp);
}

void ScreenUserData::slot_on_unbun_push_button_clicked() {

  bool ok = (QMessageBox::question(
                 this, "Разблокировка", "Вы хотите снять бан с пользователя?",
                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);
  if (!ok)
    return;

  client_session_ptr_->bunUnbunUserCl(ui->loginLineEdit->text().toStdString(), false,
                              0);
}
