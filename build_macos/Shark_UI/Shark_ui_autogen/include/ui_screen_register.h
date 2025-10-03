/********************************************************************************
** Form generated from reading UI file 'screen_register.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_REGISTER_H
#define UI_SCREEN_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenRegister
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_7;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QLabel *serverStatusLabelRound;
    QSpacerItem *horizontalSpacer;
    QLabel *serverStatusLabel;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QFormLayout *formLayout;
    QLabel *label_5;
    QLineEdit *loginEdit;
    QLabel *label_2;
    QLineEdit *nameEdit;
    QLabel *label_7;
    QLineEdit *emailLineEdit;
    QLabel *label_8;
    QLineEdit *phoneLineEdit;
    QLabel *label_3;
    QLineEdit *passwordEdit;
    QLabel *label_4;
    QLineEdit *passwordConfirmEdit;
    QLabel *hintLabel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *registerPushButton;
    QSpacerItem *horizontalSpacer_9;
    QPushButton *exitPushButton;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *toLoginButton;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_5;

    void setupUi(QWidget *ScreenRegister)
    {
        if (ScreenRegister->objectName().isEmpty())
            ScreenRegister->setObjectName("ScreenRegister");
        ScreenRegister->resize(1132, 851);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScreenRegister->sizePolicy().hasHeightForWidth());
        ScreenRegister->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(ScreenRegister);
        gridLayout->setObjectName("gridLayout");
        verticalSpacer_4 = new QSpacerItem(20, 167, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 0, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(332, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_7, 1, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        label_6 = new QLabel(ScreenRegister);
        label_6->setObjectName("label_6");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);
        label_6->setMaximumSize(QSize(221, 221));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/images/shark_1.jpg")));
        label_6->setScaledContents(true);

        horizontalLayout_3->addWidget(label_6);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer_3 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        serverStatusLabelRound = new QLabel(ScreenRegister);
        serverStatusLabelRound->setObjectName("serverStatusLabelRound");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(serverStatusLabelRound->sizePolicy().hasHeightForWidth());
        serverStatusLabelRound->setSizePolicy(sizePolicy2);
        serverStatusLabelRound->setMinimumSize(QSize(16, 16));
        serverStatusLabelRound->setMaximumSize(QSize(16, 16));
        serverStatusLabelRound->setStyleSheet(QString::fromUtf8("background-color: red;\n"
"border-radius: 8px;"));

        horizontalLayout->addWidget(serverStatusLabelRound);

        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        serverStatusLabel = new QLabel(ScreenRegister);
        serverStatusLabel->setObjectName("serverStatusLabel");

        horizontalLayout->addWidget(serverStatusLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        label = new QLabel(ScreenRegister);
        label->setObjectName("label");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy3);
        QFont font;
        font.setBold(true);
        font.setItalic(true);
        label->setFont(font);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_5 = new QLabel(ScreenRegister);
        label_5->setObjectName("label_5");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_5);

        loginEdit = new QLineEdit(ScreenRegister);
        loginEdit->setObjectName("loginEdit");
        sizePolicy1.setHeightForWidth(loginEdit->sizePolicy().hasHeightForWidth());
        loginEdit->setSizePolicy(sizePolicy1);
        loginEdit->setMinimumSize(QSize(240, 0));
        loginEdit->setToolTipDuration(10000);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, loginEdit);

        label_2 = new QLabel(ScreenRegister);
        label_2->setObjectName("label_2");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_2);

        nameEdit = new QLineEdit(ScreenRegister);
        nameEdit->setObjectName("nameEdit");
        sizePolicy1.setHeightForWidth(nameEdit->sizePolicy().hasHeightForWidth());
        nameEdit->setSizePolicy(sizePolicy1);
        nameEdit->setMinimumSize(QSize(240, 0));
        nameEdit->setToolTipDuration(10000);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, nameEdit);

        label_7 = new QLabel(ScreenRegister);
        label_7->setObjectName("label_7");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_7);

        emailLineEdit = new QLineEdit(ScreenRegister);
        emailLineEdit->setObjectName("emailLineEdit");
        sizePolicy1.setHeightForWidth(emailLineEdit->sizePolicy().hasHeightForWidth());
        emailLineEdit->setSizePolicy(sizePolicy1);
        emailLineEdit->setMinimumSize(QSize(240, 0));

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, emailLineEdit);

        label_8 = new QLabel(ScreenRegister);
        label_8->setObjectName("label_8");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_8);

        phoneLineEdit = new QLineEdit(ScreenRegister);
        phoneLineEdit->setObjectName("phoneLineEdit");
        sizePolicy1.setHeightForWidth(phoneLineEdit->sizePolicy().hasHeightForWidth());
        phoneLineEdit->setSizePolicy(sizePolicy1);
        phoneLineEdit->setMinimumSize(QSize(240, 0));

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, phoneLineEdit);

        label_3 = new QLabel(ScreenRegister);
        label_3->setObjectName("label_3");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, label_3);

        passwordEdit = new QLineEdit(ScreenRegister);
        passwordEdit->setObjectName("passwordEdit");
        sizePolicy1.setHeightForWidth(passwordEdit->sizePolicy().hasHeightForWidth());
        passwordEdit->setSizePolicy(sizePolicy1);
        passwordEdit->setMinimumSize(QSize(240, 0));
        passwordEdit->setToolTipDuration(10000);
        passwordEdit->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit);

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, passwordEdit);

        label_4 = new QLabel(ScreenRegister);
        label_4->setObjectName("label_4");

        formLayout->setWidget(6, QFormLayout::ItemRole::LabelRole, label_4);

        passwordConfirmEdit = new QLineEdit(ScreenRegister);
        passwordConfirmEdit->setObjectName("passwordConfirmEdit");
        sizePolicy1.setHeightForWidth(passwordConfirmEdit->sizePolicy().hasHeightForWidth());
        passwordConfirmEdit->setSizePolicy(sizePolicy1);
        passwordConfirmEdit->setMinimumSize(QSize(240, 0));
        passwordConfirmEdit->setToolTipDuration(10000);
        passwordConfirmEdit->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit);

        formLayout->setWidget(6, QFormLayout::ItemRole::FieldRole, passwordConfirmEdit);


        verticalLayout->addLayout(formLayout);

        hintLabel = new QLabel(ScreenRegister);
        hintLabel->setObjectName("hintLabel");
        sizePolicy1.setHeightForWidth(hintLabel->sizePolicy().hasHeightForWidth());
        hintLabel->setSizePolicy(sizePolicy1);
        hintLabel->setMinimumSize(QSize(400, 32));
        hintLabel->setMaximumSize(QSize(400, 32));
        hintLabel->setWordWrap(true);

        verticalLayout->addWidget(hintLabel);

        verticalSpacer = new QSpacerItem(17, 37, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        registerPushButton = new QPushButton(ScreenRegister);
        registerPushButton->setObjectName("registerPushButton");

        horizontalLayout_2->addWidget(registerPushButton);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_9);

        exitPushButton = new QPushButton(ScreenRegister);
        exitPushButton->setObjectName("exitPushButton");

        horizontalLayout_2->addWidget(exitPushButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        toLoginButton = new QPushButton(ScreenRegister);
        toLoginButton->setObjectName("toLoginButton");

        horizontalLayout_2->addWidget(toLoginButton);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 1, 1, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(332, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_8, 1, 2, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 166, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_5, 2, 1, 1, 1);

        QWidget::setTabOrder(loginEdit, nameEdit);
        QWidget::setTabOrder(nameEdit, emailLineEdit);
        QWidget::setTabOrder(emailLineEdit, phoneLineEdit);
        QWidget::setTabOrder(phoneLineEdit, passwordEdit);
        QWidget::setTabOrder(passwordEdit, passwordConfirmEdit);
        QWidget::setTabOrder(passwordConfirmEdit, registerPushButton);
        QWidget::setTabOrder(registerPushButton, exitPushButton);
        QWidget::setTabOrder(exitPushButton, toLoginButton);

        retranslateUi(ScreenRegister);

        QMetaObject::connectSlotsByName(ScreenRegister);
    } // setupUi

    void retranslateUi(QWidget *ScreenRegister)
    {
        ScreenRegister->setWindowTitle(QCoreApplication::translate("ScreenRegister", "\320\240\320\265\320\263\320\270\321\201\321\202\321\200\320\260\321\206\320\270\321\217 \320\275\320\276\320\262\320\276\320\263\320\276 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217", nullptr));
        label_6->setText(QString());
        serverStatusLabelRound->setText(QString());
        serverStatusLabel->setText(QCoreApplication::translate("ScreenRegister", "server offline", nullptr));
        label->setText(QCoreApplication::translate("ScreenRegister", "SHARK - messanger - \321\200\320\265\320\263\320\270\321\201\321\202\321\200\320\260\321\206\320\270\321\217", nullptr));
        label_5->setText(QCoreApplication::translate("ScreenRegister", "\320\233\320\276\320\263\320\270\320\275:", nullptr));
#if QT_CONFIG(tooltip)
        loginEdit->setToolTip(QCoreApplication::translate("ScreenRegister", "\320\235\320\265 \320\274\320\265\320\275\320\265\320\265 5 \320\270 \320\275\320\265 \320\261\320\276\320\273\320\265\320\265 20 \321\201\320\270\320\274\320\262\320\276\320\273\320\276\320\262, \321\202\320\276\320\273\321\214\320\272\320\276 \320\260\320\275\320\263\320\273\320\270\320\271\321\201\320\272\320\270\320\265 \320\261\321\203\320\272\320\262\321\213 \320\270 \321\206\320\270\321\204\321\200\321\213", nullptr));
#endif // QT_CONFIG(tooltip)
        label_2->setText(QCoreApplication::translate("ScreenRegister", "\320\230\320\274\321\217 \320\262 \321\207\320\260\321\202\320\265:", nullptr));
#if QT_CONFIG(tooltip)
        nameEdit->setToolTip(QCoreApplication::translate("ScreenRegister", "\320\235\320\265 \320\274\320\265\320\275\320\265\320\265 2 \320\270 \320\275\320\265 \320\261\320\276\320\273\320\265\320\265 20 \321\201\320\270\320\274\320\262\320\276\320\273\320\276\320\262, \321\202\320\276\320\273\321\214\320\272\320\276 \320\260\320\275\320\263\320\273\320\270\320\271\321\201\320\272\320\270\320\265 \320\261\321\203\320\272\320\262\321\213 \320\270 \321\206\320\270\321\204\321\200\321\213", nullptr));
#endif // QT_CONFIG(tooltip)
        label_7->setText(QCoreApplication::translate("ScreenRegister", "Email:", nullptr));
        label_8->setText(QCoreApplication::translate("ScreenRegister", "\320\242\320\265\320\273\320\265\321\204\320\276\320\275:", nullptr));
        label_3->setText(QCoreApplication::translate("ScreenRegister", "\320\237\320\260\321\200\320\276\320\273\321\214:", nullptr));
#if QT_CONFIG(tooltip)
        passwordEdit->setToolTip(QCoreApplication::translate("ScreenRegister", "\320\235\320\265 \320\274\320\265\320\275\320\265\320\265 5 \320\270 \320\275\320\265 \320\261\320\276\320\273\320\265\320\265 20 \321\201\320\270\320\274\320\262\320\276\320\273\320\276\320\262, \320\234\320\275\320\270\320\274\321\203\320\274 \320\276\320\264\320\275\320\260 \320\267\320\260\320\263\320\273\320\260\320\262\320\275\320\260\321\217 \320\270 \320\276\320\264\320\275\320\260 \321\206\320\270\321\204\321\200\320\260 (\321\202\320\276\320\273\321\214\320\272\320\276 \320\273\320\260\321\202\320\270\320\275\321\201\320\272\320\270\320\265 \320\261\321\203\320\272\320\262\321\213)", nullptr));
#endif // QT_CONFIG(tooltip)
        label_4->setText(QCoreApplication::translate("ScreenRegister", "\320\237\320\260\321\200\320\276\320\273\321\214 \320\265\321\211\320\265 \321\200\320\260\320\267:", nullptr));
#if QT_CONFIG(tooltip)
        passwordConfirmEdit->setToolTip(QCoreApplication::translate("ScreenRegister", "\320\237\320\276\320\262\321\202\320\276\321\200\320\270\321\202\320\265 \320\262\320\262\320\276\320\264 \320\277\320\260\321\200\320\276\320\273\321\217", nullptr));
#endif // QT_CONFIG(tooltip)
        hintLabel->setText(QString());
        registerPushButton->setText(QCoreApplication::translate("ScreenRegister", "\320\227\320\260\321\200\320\265\320\263\320\270\321\201\321\202\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        exitPushButton->setText(QCoreApplication::translate("ScreenRegister", "\320\222\321\213\321\205\320\276\320\264", nullptr));
        toLoginButton->setText(QCoreApplication::translate("ScreenRegister", "\320\262\320\276\320\271\321\202\320\270  \321\201 \320\273\320\276\320\263\320\270\320\275\320\276\320\274", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenRegister: public Ui_ScreenRegister {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_REGISTER_H
