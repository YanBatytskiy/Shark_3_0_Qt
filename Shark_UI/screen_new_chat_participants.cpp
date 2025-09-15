#include "screen_new_chat_participants.h"
#include "ui_screen_new_chat_participants.h"

#include <QObject>

ScreenNewChatParticipants::ScreenNewChatParticipants(QWidget *parent) : QWidget(parent),
                                                                        ui(new Ui::ScreenNewChatParticipants) {
  ui->setupUi(this);
  this->setVisible(false);

  _participantsListModel = new QStringListModel(this);

  ui->screenUserDataNewChatUsersList->setModel(_participantsListModel);
}

ScreenNewChatParticipants::~ScreenNewChatParticipants() {
  delete ui;
}

void ScreenNewChatParticipants::slotCollectParticipantsForNewChat() {
  this->setVisible(true);
  this->setEnabled(true);
  _participantsListModel->setStringList({});
}

void ScreenNewChatParticipants::slotAddContactToParticipantsList(const QString &value) {

  const auto quantity = _participantsListModel->rowCount();

  for (const auto &line : _participantsListModel->stringList()) {
    if (line == value)
      return;
    ;
  }

  _participantsListModel->insertRow(quantity);

  const auto &idx = _participantsListModel->index(_participantsListModel->rowCount() - 1);

  _participantsListModel->setData(idx, value);
}

void ScreenNewChatParticipants::on_screenNewChatDeleteContactPushButton_clicked() {
  const auto &selectModel = ui->screenUserDataNewChatUsersList->selectionModel();

  const auto &idx = selectModel->currentIndex();

  if (idx.isValid()) {
    _participantsListModel->removeRow(idx.row());
  }
}

void ScreenNewChatParticipants::on_screenUserDataCancelNewChatPushButton_clicked() {
  this->setVisible(false);
  this->setEnabled(false);

  _participantsListModel->setStringList({});
  emit signalCancelNewChat();
}
