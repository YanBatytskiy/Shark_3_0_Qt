/********************************************************************************
** Form generated from reading UI file 'screen_login.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_LOGIN_H
#define UI_SCREEN_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenLogin
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacerMainTop;
    QSpacerItem *verticalSpacerMainLeft;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_7;
    QHBoxLayout *horizontalLayout;
    QLabel *serverStatusLabelRound;
    QSpacerItem *horizontalSpacer;
    QLabel *serverStatusLabel;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_4;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *loginEdit;
    QLabel *label_2;
    QLineEdit *passwordEdit;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_2;
    QDialogButtonBox *loginButtonBox;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *registerModeButton;
    QSpacerItem *verticalSpacer_8;
    QPushButton *baseReInitialisationPushButton;
    QPushButton *logFileClearPushButton;
    QPushButton *lookLogLastLinePushButton;
    QPushButton *lookLogSeveralLinePushButton;
    QSpacerItem *verticalSpacer_6;
    QLabel *serverDataLabel;
    QSpacerItem *verticalSpacerMairight;
    QSpacerItem *verticalSpacerMainBottom;

    void setupUi(QWidget *ScreenLogin)
    {
        if (ScreenLogin->objectName().isEmpty())
            ScreenLogin->setObjectName("ScreenLogin");
        ScreenLogin->resize(783, 768);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScreenLogin->sizePolicy().hasHeightForWidth());
        ScreenLogin->setSizePolicy(sizePolicy);
        ScreenLogin->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(ScreenLogin);
        gridLayout->setObjectName("gridLayout");
        verticalSpacerMainTop = new QSpacerItem(20, 77, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacerMainTop, 0, 1, 1, 1);

        verticalSpacerMainLeft = new QSpacerItem(326, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(verticalSpacerMainLeft, 1, 0, 1, 1);

        frame = new QFrame(ScreenLogin);
        frame->setObjectName("frame");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        frame->setLineWidth(2);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        label_4 = new QLabel(frame);
        label_4->setObjectName("label_4");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        label_4->setMinimumSize(QSize(221, 221));
        label_4->setMaximumSize(QSize(221, 221));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/images/shark_1.jpg")));
        label_4->setScaledContents(true);

        horizontalLayout_5->addWidget(label_4);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalSpacer_7 = new QSpacerItem(20, 13, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_7);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        serverStatusLabelRound = new QLabel(frame);
        serverStatusLabelRound->setObjectName("serverStatusLabelRound");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(serverStatusLabelRound->sizePolicy().hasHeightForWidth());
        serverStatusLabelRound->setSizePolicy(sizePolicy3);
        serverStatusLabelRound->setMinimumSize(QSize(16, 16));
        serverStatusLabelRound->setMaximumSize(QSize(16, 16));
        serverStatusLabelRound->setStyleSheet(QString::fromUtf8("background-color: red;\n"
"border-radius: 8px;"));

        horizontalLayout->addWidget(serverStatusLabelRound);

        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        serverStatusLabel = new QLabel(frame);
        serverStatusLabel->setObjectName("serverStatusLabel");
        sizePolicy3.setHeightForWidth(serverStatusLabel->sizePolicy().hasHeightForWidth());
        serverStatusLabel->setSizePolicy(sizePolicy3);
        serverStatusLabel->setAutoFillBackground(false);
        serverStatusLabel->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(serverStatusLabel);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        sizePolicy3.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy3);
        QFont font;
        font.setBold(true);
        font.setItalic(true);
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(label_3);

        horizontalSpacer_7 = new QSpacerItem(2, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_4 = new QSpacerItem(20, 13, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_4);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setStyleSheet(QString::fromUtf8(""));

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        loginEdit = new QLineEdit(frame);
        loginEdit->setObjectName("loginEdit");
        loginEdit->setToolTipDuration(10000);
        loginEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(231, 230, 232);\n"
"border-color: rgb(0, 0, 0);"));

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, loginEdit);

        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setStyleSheet(QString::fromUtf8(""));

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        passwordEdit = new QLineEdit(frame);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(231, 230, 232);\n"
"border-color: rgb(0, 0, 0);"));
        passwordEdit->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, passwordEdit);


        verticalLayout->addLayout(formLayout);

        verticalSpacer_5 = new QSpacerItem(20, 13, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        loginButtonBox = new QDialogButtonBox(frame);
        loginButtonBox->setObjectName("loginButtonBox");
        sizePolicy2.setHeightForWidth(loginButtonBox->sizePolicy().hasHeightForWidth());
        loginButtonBox->setSizePolicy(sizePolicy2);
        loginButtonBox->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        loginButtonBox->setAutoFillBackground(false);
        loginButtonBox->setStyleSheet(QString::fromUtf8(""));
        loginButtonBox->setStandardButtons(QDialogButtonBox::StandardButton::Close|QDialogButtonBox::StandardButton::Ok);
        loginButtonBox->setCenterButtons(true);

        horizontalLayout_2->addWidget(loginButtonBox);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        registerModeButton = new QPushButton(frame);
        registerModeButton->setObjectName("registerModeButton");
        sizePolicy2.setHeightForWidth(registerModeButton->sizePolicy().hasHeightForWidth());
        registerModeButton->setSizePolicy(sizePolicy2);
        registerModeButton->setStyleSheet(QString::fromUtf8(""));
        registerModeButton->setAutoDefault(false);

        horizontalLayout_2->addWidget(registerModeButton);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_8 = new QSpacerItem(20, 13, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_8);

        baseReInitialisationPushButton = new QPushButton(frame);
        baseReInitialisationPushButton->setObjectName("baseReInitialisationPushButton");

        verticalLayout->addWidget(baseReInitialisationPushButton);

        logFileClearPushButton = new QPushButton(frame);
        logFileClearPushButton->setObjectName("logFileClearPushButton");

        verticalLayout->addWidget(logFileClearPushButton);

        lookLogLastLinePushButton = new QPushButton(frame);
        lookLogLastLinePushButton->setObjectName("lookLogLastLinePushButton");

        verticalLayout->addWidget(lookLogLastLinePushButton);

        lookLogSeveralLinePushButton = new QPushButton(frame);
        lookLogSeveralLinePushButton->setObjectName("lookLogSeveralLinePushButton");

        verticalLayout->addWidget(lookLogSeveralLinePushButton);

        verticalSpacer_6 = new QSpacerItem(20, 13, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_6);

        serverDataLabel = new QLabel(frame);
        serverDataLabel->setObjectName("serverDataLabel");
        sizePolicy.setHeightForWidth(serverDataLabel->sizePolicy().hasHeightForWidth());
        serverDataLabel->setSizePolicy(sizePolicy);
        serverDataLabel->setMaximumSize(QSize(353, 16777215));
        serverDataLabel->setStyleSheet(QString::fromUtf8(""));
        serverDataLabel->setWordWrap(true);

        verticalLayout->addWidget(serverDataLabel);


        gridLayout->addWidget(frame, 1, 1, 1, 1);

        verticalSpacerMairight = new QSpacerItem(325, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(verticalSpacerMairight, 1, 2, 1, 1);

        verticalSpacerMainBottom = new QSpacerItem(20, 165, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacerMainBottom, 2, 1, 1, 1);

        QWidget::setTabOrder(loginEdit, passwordEdit);
        QWidget::setTabOrder(passwordEdit, registerModeButton);

        retranslateUi(ScreenLogin);

        QMetaObject::connectSlotsByName(ScreenLogin);
    } // setupUi

    void retranslateUi(QWidget *ScreenLogin)
    {
        ScreenLogin->setWindowTitle(QCoreApplication::translate("ScreenLogin", "\320\222\321\205\320\276\320\264 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217", nullptr));
        label_4->setText(QString());
        serverStatusLabelRound->setText(QString());
        serverStatusLabel->setText(QCoreApplication::translate("ScreenLogin", "server offline", nullptr));
        label_3->setText(QCoreApplication::translate("ScreenLogin", "SHARK - messanger - \320\262\321\205\320\276\320\264", nullptr));
        label->setText(QCoreApplication::translate("ScreenLogin", "\320\230\320\274\321\217 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217:", nullptr));
#if QT_CONFIG(tooltip)
        loginEdit->setToolTip(QCoreApplication::translate("ScreenLogin", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\273\320\276\320\263\320\270\320\275", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        loginEdit->setStatusTip(QString());
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(whatsthis)
        loginEdit->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        label_2->setText(QCoreApplication::translate("ScreenLogin", "\320\237\320\260\321\200\320\276\320\273\321\214:", nullptr));
        registerModeButton->setText(QCoreApplication::translate("ScreenLogin", "Register new user", nullptr));
        baseReInitialisationPushButton->setText(QCoreApplication::translate("ScreenLogin", "\320\237\320\265\321\200\320\265\320\270\320\275\320\270\321\206\320\270\320\260\320\273\320\270\320\267\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \320\261\320\260\320\267\321\203 \320\264\320\260\320\275\320\275\321\213\321\205", nullptr));
        logFileClearPushButton->setText(QCoreApplication::translate("ScreenLogin", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \321\204\320\260\320\271\320\273 \320\273\320\276\320\263\320\276\320\262", nullptr));
        lookLogLastLinePushButton->setText(QCoreApplication::translate("ScreenLogin", "\320\237\320\276\321\201\320\274\320\276\321\202\321\200\320\265\321\202\321\214 \320\277\320\276\321\201\320\273\320\265\320\264\320\275\321\216\321\216 \321\201\321\202\321\200\320\276\320\272\321\203 \321\204\320\260\320\271\320\273\320\260", nullptr));
        lookLogSeveralLinePushButton->setText(QCoreApplication::translate("ScreenLogin", "\320\237\320\276\321\201\320\274\320\276\321\202\321\200\320\265\321\202\321\214 \320\275\320\265\321\201\320\272\320\276\320\273\321\214\320\272\320\276 \320\277\320\276\321\201\320\273\320\265\320\264\320\275\320\270\321\205 \321\201\321\202\321\200\320\276\320\272 \320\273\320\276\320\263\320\260", nullptr));
        serverDataLabel->setText(QCoreApplication::translate("ScreenLogin", "\320\264\320\260\321\202\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenLogin: public Ui_ScreenLogin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_LOGIN_H
