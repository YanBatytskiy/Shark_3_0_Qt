/********************************************************************************
** Form generated from reading UI file 'screen_user_data.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_USER_DATA_H
#define UI_SCREEN_USER_DATA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "screen_chat_list.h"
#include "screen_chatting.h"
#include "screen_new_chat_participants.h"

QT_BEGIN_NAMESPACE

class Ui_ScreenUserData
{
public:
    QHBoxLayout *horizontalLayout_3;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *saveCancelHorizontalLayout;
    QLabel *changePasswordLabel_2;
    QFormLayout *formLayout_3;
    QLabel *label;
    QLineEdit *loginLineEdit;
    QLabel *label_2;
    QLineEdit *nameLineEdit;
    QLabel *label_4;
    QLineEdit *emailLineEdit;
    QLabel *label_3;
    QLineEdit *phoneLineEdit;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *banPushButton;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *blockPushButton;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout;
    QPushButton *unBunPushButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *unblockPushButton;
    QSpacerItem *verticalSpacer_2;
    QLabel *bunnedToDateUserLabel;
    QSpacerItem *verticalSpacer;
    QLabel *blockedUserLabel;
    QSpacerItem *verticalSpacer_5;
    QLabel *reasonDisableabel;
    QSpacerItem *verticalSpacer_9;
    ScreenNewChatParticipants *ScreenNewChatParticipantsWidget;
    QSpacerItem *verticalSpacer_11;
    ScreenChatList *ScreenUserDataChatsListWidget;
    ScreenChatting *ScreenUserDataMessagesListWidget;

    void setupUi(QWidget *ScreenUserData)
    {
        if (ScreenUserData->objectName().isEmpty())
            ScreenUserData->setObjectName("ScreenUserData");
        ScreenUserData->resize(1193, 989);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScreenUserData->sizePolicy().hasHeightForWidth());
        ScreenUserData->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(ScreenUserData);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        widget = new QWidget(ScreenUserData);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMinimumSize(QSize(320, 0));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        saveCancelHorizontalLayout = new QHBoxLayout();
        saveCancelHorizontalLayout->setObjectName("saveCancelHorizontalLayout");
        changePasswordLabel_2 = new QLabel(widget);
        changePasswordLabel_2->setObjectName("changePasswordLabel_2");
        sizePolicy1.setHeightForWidth(changePasswordLabel_2->sizePolicy().hasHeightForWidth());
        changePasswordLabel_2->setSizePolicy(sizePolicy1);
        QFont font;
        font.setBold(true);
        changePasswordLabel_2->setFont(font);
        changePasswordLabel_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        saveCancelHorizontalLayout->addWidget(changePasswordLabel_2);


        verticalLayout->addLayout(saveCancelHorizontalLayout);

        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName("formLayout_3");
        label = new QLabel(widget);
        label->setObjectName("label");

        formLayout_3->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        loginLineEdit = new QLineEdit(widget);
        loginLineEdit->setObjectName("loginLineEdit");
        loginLineEdit->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(loginLineEdit->sizePolicy().hasHeightForWidth());
        loginLineEdit->setSizePolicy(sizePolicy2);
        loginLineEdit->setMinimumSize(QSize(250, 0));
        loginLineEdit->setAutoFillBackground(false);
        loginLineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 238, 240);\n"
"border-color: rgb(0, 0, 0);"));
        loginLineEdit->setReadOnly(true);

        formLayout_3->setWidget(0, QFormLayout::ItemRole::FieldRole, loginLineEdit);

        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");

        formLayout_3->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        nameLineEdit = new QLineEdit(widget);
        nameLineEdit->setObjectName("nameLineEdit");
        nameLineEdit->setEnabled(true);
        sizePolicy2.setHeightForWidth(nameLineEdit->sizePolicy().hasHeightForWidth());
        nameLineEdit->setSizePolicy(sizePolicy2);
        nameLineEdit->setMinimumSize(QSize(250, 0));
        nameLineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 238, 240);\n"
"border-color: rgb(0, 0, 0);"));
        nameLineEdit->setReadOnly(true);

        formLayout_3->setWidget(1, QFormLayout::ItemRole::FieldRole, nameLineEdit);

        label_4 = new QLabel(widget);
        label_4->setObjectName("label_4");

        formLayout_3->setWidget(2, QFormLayout::ItemRole::LabelRole, label_4);

        emailLineEdit = new QLineEdit(widget);
        emailLineEdit->setObjectName("emailLineEdit");
        emailLineEdit->setEnabled(true);
        sizePolicy2.setHeightForWidth(emailLineEdit->sizePolicy().hasHeightForWidth());
        emailLineEdit->setSizePolicy(sizePolicy2);
        emailLineEdit->setMinimumSize(QSize(250, 0));
        emailLineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 238, 240);\n"
"border-color: rgb(0, 0, 0);"));
        emailLineEdit->setReadOnly(true);

        formLayout_3->setWidget(2, QFormLayout::ItemRole::FieldRole, emailLineEdit);

        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");

        formLayout_3->setWidget(3, QFormLayout::ItemRole::LabelRole, label_3);

        phoneLineEdit = new QLineEdit(widget);
        phoneLineEdit->setObjectName("phoneLineEdit");
        phoneLineEdit->setEnabled(true);
        sizePolicy2.setHeightForWidth(phoneLineEdit->sizePolicy().hasHeightForWidth());
        phoneLineEdit->setSizePolicy(sizePolicy2);
        phoneLineEdit->setMinimumSize(QSize(250, 0));
        phoneLineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 238, 240);\n"
"border-color: rgb(0, 0, 0);"));
        phoneLineEdit->setReadOnly(true);

        formLayout_3->setWidget(3, QFormLayout::ItemRole::FieldRole, phoneLineEdit);


        verticalLayout->addLayout(formLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        banPushButton = new QPushButton(widget);
        banPushButton->setObjectName("banPushButton");
        sizePolicy2.setHeightForWidth(banPushButton->sizePolicy().hasHeightForWidth());
        banPushButton->setSizePolicy(sizePolicy2);
        banPushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 183, 195);"));

        horizontalLayout_2->addWidget(banPushButton);

        horizontalSpacer_6 = new QSpacerItem(18, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        blockPushButton = new QPushButton(widget);
        blockPushButton->setObjectName("blockPushButton");
        sizePolicy2.setHeightForWidth(blockPushButton->sizePolicy().hasHeightForWidth());
        blockPushButton->setSizePolicy(sizePolicy2);
        blockPushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 183, 195);"));

        horizontalLayout_2->addWidget(blockPushButton);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_6 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_6);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        unBunPushButton = new QPushButton(widget);
        unBunPushButton->setObjectName("unBunPushButton");
        sizePolicy2.setHeightForWidth(unBunPushButton->sizePolicy().hasHeightForWidth());
        unBunPushButton->setSizePolicy(sizePolicy2);
        unBunPushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 169);"));

        horizontalLayout->addWidget(unBunPushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        unblockPushButton = new QPushButton(widget);
        unblockPushButton->setObjectName("unblockPushButton");
        sizePolicy2.setHeightForWidth(unblockPushButton->sizePolicy().hasHeightForWidth());
        unblockPushButton->setSizePolicy(sizePolicy2);
        unblockPushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(175, 255, 174);"));

        horizontalLayout->addWidget(unblockPushButton);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        bunnedToDateUserLabel = new QLabel(widget);
        bunnedToDateUserLabel->setObjectName("bunnedToDateUserLabel");
        bunnedToDateUserLabel->setStyleSheet(QString::fromUtf8("color: rgb(192, 28, 40);"));

        verticalLayout->addWidget(bunnedToDateUserLabel);

        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        blockedUserLabel = new QLabel(widget);
        blockedUserLabel->setObjectName("blockedUserLabel");
        blockedUserLabel->setStyleSheet(QString::fromUtf8("color: rgb(192, 28, 40);"));

        verticalLayout->addWidget(blockedUserLabel);

        verticalSpacer_5 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_5);

        reasonDisableabel = new QLabel(widget);
        reasonDisableabel->setObjectName("reasonDisableabel");
        reasonDisableabel->setStyleSheet(QString::fromUtf8("color: rgb(192, 28, 40);"));

        verticalLayout->addWidget(reasonDisableabel);

        verticalSpacer_9 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_9);

        ScreenNewChatParticipantsWidget = new ScreenNewChatParticipants(widget);
        ScreenNewChatParticipantsWidget->setObjectName("ScreenNewChatParticipantsWidget");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(ScreenNewChatParticipantsWidget->sizePolicy().hasHeightForWidth());
        ScreenNewChatParticipantsWidget->setSizePolicy(sizePolicy3);
        ScreenNewChatParticipantsWidget->setMinimumSize(QSize(0, 450));
        ScreenNewChatParticipantsWidget->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout->addWidget(ScreenNewChatParticipantsWidget);

        verticalSpacer_11 = new QSpacerItem(20, 78, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_11);


        horizontalLayout_3->addWidget(widget);

        ScreenUserDataChatsListWidget = new ScreenChatList(ScreenUserData);
        ScreenUserDataChatsListWidget->setObjectName("ScreenUserDataChatsListWidget");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(ScreenUserDataChatsListWidget->sizePolicy().hasHeightForWidth());
        ScreenUserDataChatsListWidget->setSizePolicy(sizePolicy4);
        ScreenUserDataChatsListWidget->setMinimumSize(QSize(320, 0));
        ScreenUserDataChatsListWidget->setMaximumSize(QSize(320, 16777215));

        horizontalLayout_3->addWidget(ScreenUserDataChatsListWidget);

        ScreenUserDataMessagesListWidget = new ScreenChatting(ScreenUserData);
        ScreenUserDataMessagesListWidget->setObjectName("ScreenUserDataMessagesListWidget");
        sizePolicy.setHeightForWidth(ScreenUserDataMessagesListWidget->sizePolicy().hasHeightForWidth());
        ScreenUserDataMessagesListWidget->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(ScreenUserDataMessagesListWidget);


        retranslateUi(ScreenUserData);

        QMetaObject::connectSlotsByName(ScreenUserData);
    } // setupUi

    void retranslateUi(QWidget *ScreenUserData)
    {
        ScreenUserData->setWindowTitle(QCoreApplication::translate("ScreenUserData", "Form", nullptr));
        changePasswordLabel_2->setText(QCoreApplication::translate("ScreenUserData", "\320\224\320\260\320\275\320\275\321\213\320\265 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217", nullptr));
        label->setText(QCoreApplication::translate("ScreenUserData", "\320\233\320\276\320\263\320\270\320\275:", nullptr));
        label_2->setText(QCoreApplication::translate("ScreenUserData", "\320\230\320\274\321\217:", nullptr));
        label_4->setText(QCoreApplication::translate("ScreenUserData", "Email:", nullptr));
#if QT_CONFIG(tooltip)
        emailLineEdit->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        label_3->setText(QCoreApplication::translate("ScreenUserData", "\320\242\320\265\320\273\320\265\321\204\320\276\320\275:", nullptr));
        banPushButton->setText(QCoreApplication::translate("ScreenUserData", "\320\222\321\213\320\264\320\260\321\202\321\214 \320\261\320\260\320\275 \320\264\320\276...", nullptr));
        blockPushButton->setText(QCoreApplication::translate("ScreenUserData", "\320\227\320\260\320\261\320\273\320\276\320\272\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        unBunPushButton->setText(QCoreApplication::translate("ScreenUserData", "\320\241\320\275\321\217\321\202\321\214 \320\261\320\260\320\275", nullptr));
        unblockPushButton->setText(QCoreApplication::translate("ScreenUserData", "\320\240\320\260\320\267\320\261\320\273\320\276\320\272\320\270\321\200\320\276\320\262\320\260\321\202\321\214", nullptr));
        bunnedToDateUserLabel->setText(QCoreApplication::translate("ScreenUserData", "\320\232\320\276\320\275\321\202\320\260\320\272\321\202 \320\267\320\260\320\261\320\260\320\275\320\265\320\275 \320\264\320\276:", nullptr));
        blockedUserLabel->setText(QCoreApplication::translate("ScreenUserData", "\320\232\320\276\320\275\321\202\320\260\320\272\321\202 \320\267\320\260\320\261\320\273\320\276\320\272\320\270\321\200\320\276\320\262\320\260\320\275.", nullptr));
        reasonDisableabel->setText(QCoreApplication::translate("ScreenUserData", "\320\237\321\200\320\270\321\207\320\270\320\275\320\260 \320\261\320\273\320\276\320\272\320\270\321\200\320\276\320\262\320\272\320\270:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenUserData: public Ui_ScreenUserData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_USER_DATA_H
