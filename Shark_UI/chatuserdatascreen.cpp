#include "chatuserdatascreen.h"
#include "ui_chatuserdatascreen.h"
#include <QAbstractTextDocumentLayout>


chatListScreen::chatListScreen(QWidget *parent) : QWidget(parent), ui(new Ui::chatListScreen) {
  ui->setupUi(this);

  ui->textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  // ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // функция пересчёта высоты
  auto te = ui->textEdit;
  auto updateHeight = [te]{
    // высота документа в пикселях
    const qreal docH = te->document()->documentLayout()->documentSize().height();
    const int lineH  = te->fontMetrics().height();

           // рамки/поля
    const auto m = te->contentsMargins();
    const int frame = te->frameWidth() * 2;

           // не меньше одной строки
    int h = qMax<int>(std::ceil(docH), lineH) + m.top() + m.bottom() + frame;

    te->setFixedHeight(h);
  };


  // обновлять при любом изменении текста/форматирования
  QObject::connect(te->document(), &QTextDocument::contentsChanged, te, updateHeight);

  te->setMaximumHeight(240); // кап по высоте, при переполнении появится скролл

  ui->textEdit->setMaximumHeight(QWIDGETSIZE_MAX);

  // стартовая высота = одна строка
  updateHeight();
}

chatListScreen::~chatListScreen() { delete ui; }
