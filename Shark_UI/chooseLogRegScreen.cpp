#include "chooseLogRegScreen.h"
#include "ui_chooseLogRegScreen.h"

chooseLogRegScreen::chooseLogRegScreen(std::shared_ptr<ClientSession> sessionPtr, QWidget *parent)
    : QDialog(parent), ui(new Ui::chooseLogRegScreen), _sessionPtr(std::move(sessionPtr)){

  ui->setupUi(this);




}

chooseLogRegScreen::~chooseLogRegScreen() { delete ui; }
