/********************************************************************************
** Form generated from reading UI file 'screen_logger_form.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_LOGGER_FORM_H
#define UI_SCREEN_LOGGER_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenLoggerForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QTableView *logTableView;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ExitPushButton;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *ScreenLoggerForm)
    {
        if (ScreenLoggerForm->objectName().isEmpty())
            ScreenLoggerForm->setObjectName("ScreenLoggerForm");
        ScreenLoggerForm->resize(1575, 693);
        verticalLayout_2 = new QVBoxLayout(ScreenLoggerForm);
        verticalLayout_2->setObjectName("verticalLayout_2");
        frame = new QFrame(ScreenLoggerForm);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        frame->setLineWidth(2);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(frame);
        label->setObjectName("label");
        QFont font;
        font.setBold(true);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        logTableView = new QTableView(frame);
        logTableView->setObjectName("logTableView");

        verticalLayout->addWidget(logTableView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        ExitPushButton = new QPushButton(frame);
        ExitPushButton->setObjectName("ExitPushButton");

        horizontalLayout_2->addWidget(ExitPushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(frame);


        retranslateUi(ScreenLoggerForm);

        QMetaObject::connectSlotsByName(ScreenLoggerForm);
    } // setupUi

    void retranslateUi(QWidget *ScreenLoggerForm)
    {
        ScreenLoggerForm->setWindowTitle(QCoreApplication::translate("ScreenLoggerForm", "Form", nullptr));
        label->setText(QCoreApplication::translate("ScreenLoggerForm", "\320\226\321\203\321\200\320\275\320\260\320\273 \321\201\320\276\320\261\321\213\321\202\320\270\320\271", nullptr));
        ExitPushButton->setText(QCoreApplication::translate("ScreenLoggerForm", "\320\267\320\260\320\272\321\200\321\213\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenLoggerForm: public Ui_ScreenLoggerForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_LOGGER_FORM_H
