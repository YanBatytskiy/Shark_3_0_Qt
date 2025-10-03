/********************************************************************************
** Form generated from reading UI file 'screen_main_work.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_MAIN_WORK_H
#define UI_SCREEN_MAIN_WORK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "screen_chat_list.h"
#include "screen_chatting.h"
#include "screen_user_data.h"

QT_BEGIN_NAMESPACE

class Ui_ScreenMainWork
{
public:
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *serverStatusLabelRound;
    QLabel *serverStatusLabel;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *ProfilePushButton;
    QHBoxLayout *horizontalLayout_2;
    QLabel *loginLabel;
    QSpacerItem *horizontalSpacer;
    QLabel *nameLabel;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *findLineEdit;
    QSpacerItem *verticalSpacer;
    QTabWidget *mainWorkChatUserTabWidget;
    ScreenChatList *mainWorkTabChatsList;
    QWidget *mainWorkTabUsersList;
    QVBoxLayout *verticalLayout;
    QLabel *addressBookLabel;
    QHBoxLayout *horizontalLayout_5;
    QListView *mainWorkUsersList;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *addUserToChatPushButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *createNewChatPushButton;
    QPushButton *logOutPushButton;
    QStackedWidget *mainWorkRightStackedWidget;
    ScreenChatting *mainWorkPageChatting;
    ScreenUserData *mainWorkPageUserDataView;

    void setupUi(QWidget *ScreenMainWork)
    {
        if (ScreenMainWork->objectName().isEmpty())
            ScreenMainWork->setObjectName("ScreenMainWork");
        ScreenMainWork->resize(1044, 812);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScreenMainWork->sizePolicy().hasHeightForWidth());
        ScreenMainWork->setSizePolicy(sizePolicy);
        horizontalLayout_6 = new QHBoxLayout(ScreenMainWork);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        serverStatusLabelRound = new QLabel(ScreenMainWork);
        serverStatusLabelRound->setObjectName("serverStatusLabelRound");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(serverStatusLabelRound->sizePolicy().hasHeightForWidth());
        serverStatusLabelRound->setSizePolicy(sizePolicy1);
        serverStatusLabelRound->setMinimumSize(QSize(16, 16));
        serverStatusLabelRound->setMaximumSize(QSize(16, 16));
        serverStatusLabelRound->setStyleSheet(QString::fromUtf8("background-color: red;\n"
"border-radius: 8px;"));

        horizontalLayout->addWidget(serverStatusLabelRound);

        serverStatusLabel = new QLabel(ScreenMainWork);
        serverStatusLabel->setObjectName("serverStatusLabel");
        sizePolicy1.setHeightForWidth(serverStatusLabel->sizePolicy().hasHeightForWidth());
        serverStatusLabel->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(serverStatusLabel);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        ProfilePushButton = new QPushButton(ScreenMainWork);
        ProfilePushButton->setObjectName("ProfilePushButton");
        ProfilePushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(109, 182, 136);"));

        horizontalLayout->addWidget(ProfilePushButton);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        loginLabel = new QLabel(ScreenMainWork);
        loginLabel->setObjectName("loginLabel");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(loginLabel->sizePolicy().hasHeightForWidth());
        loginLabel->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(loginLabel);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        nameLabel = new QLabel(ScreenMainWork);
        nameLabel->setObjectName("nameLabel");
        sizePolicy2.setHeightForWidth(nameLabel->sizePolicy().hasHeightForWidth());
        nameLabel->setSizePolicy(sizePolicy2);
        nameLabel->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_2->addWidget(nameLabel);

        horizontalSpacer_5 = new QSpacerItem(10, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        findLineEdit = new QLineEdit(ScreenMainWork);
        findLineEdit->setObjectName("findLineEdit");
        findLineEdit->setEnabled(false);
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(findLineEdit->sizePolicy().hasHeightForWidth());
        findLineEdit->setSizePolicy(sizePolicy3);
        findLineEdit->setToolTipDuration(-5);

        horizontalLayout_3->addWidget(findLineEdit);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer);

        mainWorkChatUserTabWidget = new QTabWidget(ScreenMainWork);
        mainWorkChatUserTabWidget->setObjectName("mainWorkChatUserTabWidget");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(mainWorkChatUserTabWidget->sizePolicy().hasHeightForWidth());
        mainWorkChatUserTabWidget->setSizePolicy(sizePolicy4);
        mainWorkChatUserTabWidget->setMinimumSize(QSize(320, 0));
        mainWorkChatUserTabWidget->setMaximumSize(QSize(320, 16777215));
        mainWorkChatUserTabWidget->setElideMode(Qt::TextElideMode::ElideNone);
        mainWorkTabChatsList = new ScreenChatList();
        mainWorkTabChatsList->setObjectName("mainWorkTabChatsList");
        QSizePolicy sizePolicy5(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(mainWorkTabChatsList->sizePolicy().hasHeightForWidth());
        mainWorkTabChatsList->setSizePolicy(sizePolicy5);
        mainWorkChatUserTabWidget->addTab(mainWorkTabChatsList, QString());
        mainWorkTabUsersList = new QWidget();
        mainWorkTabUsersList->setObjectName("mainWorkTabUsersList");
        sizePolicy5.setHeightForWidth(mainWorkTabUsersList->sizePolicy().hasHeightForWidth());
        mainWorkTabUsersList->setSizePolicy(sizePolicy5);
        verticalLayout = new QVBoxLayout(mainWorkTabUsersList);
        verticalLayout->setObjectName("verticalLayout");
        addressBookLabel = new QLabel(mainWorkTabUsersList);
        addressBookLabel->setObjectName("addressBookLabel");

        verticalLayout->addWidget(addressBookLabel);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        mainWorkUsersList = new QListView(mainWorkTabUsersList);
        mainWorkUsersList->setObjectName("mainWorkUsersList");
        QSizePolicy sizePolicy6(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(mainWorkUsersList->sizePolicy().hasHeightForWidth());
        mainWorkUsersList->setSizePolicy(sizePolicy6);

        horizontalLayout_5->addWidget(mainWorkUsersList);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        addUserToChatPushButton = new QPushButton(mainWorkTabUsersList);
        addUserToChatPushButton->setObjectName("addUserToChatPushButton");
        addUserToChatPushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(164, 190, 255);"));

        horizontalLayout_4->addWidget(addUserToChatPushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        createNewChatPushButton = new QPushButton(mainWorkTabUsersList);
        createNewChatPushButton->setObjectName("createNewChatPushButton");
        createNewChatPushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(249, 240, 107);"));

        horizontalLayout_4->addWidget(createNewChatPushButton);


        verticalLayout->addLayout(horizontalLayout_4);

        mainWorkChatUserTabWidget->addTab(mainWorkTabUsersList, QString());

        verticalLayout_2->addWidget(mainWorkChatUserTabWidget);

        logOutPushButton = new QPushButton(ScreenMainWork);
        logOutPushButton->setObjectName("logOutPushButton");
        logOutPushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 195, 201);"));

        verticalLayout_2->addWidget(logOutPushButton);


        horizontalLayout_6->addLayout(verticalLayout_2);

        mainWorkRightStackedWidget = new QStackedWidget(ScreenMainWork);
        mainWorkRightStackedWidget->setObjectName("mainWorkRightStackedWidget");
        sizePolicy.setHeightForWidth(mainWorkRightStackedWidget->sizePolicy().hasHeightForWidth());
        mainWorkRightStackedWidget->setSizePolicy(sizePolicy);
        mainWorkPageChatting = new ScreenChatting();
        mainWorkPageChatting->setObjectName("mainWorkPageChatting");
        mainWorkRightStackedWidget->addWidget(mainWorkPageChatting);
        mainWorkPageUserDataView = new ScreenUserData();
        mainWorkPageUserDataView->setObjectName("mainWorkPageUserDataView");
        mainWorkRightStackedWidget->addWidget(mainWorkPageUserDataView);

        horizontalLayout_6->addWidget(mainWorkRightStackedWidget);

        horizontalLayout_6->setStretch(1, 1);

        retranslateUi(ScreenMainWork);

        mainWorkChatUserTabWidget->setCurrentIndex(1);
        mainWorkRightStackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ScreenMainWork);
    } // setupUi

    void retranslateUi(QWidget *ScreenMainWork)
    {
        ScreenMainWork->setWindowTitle(QCoreApplication::translate("ScreenMainWork", "Shark - 3.0Qt", nullptr));
        serverStatusLabelRound->setText(QString());
        serverStatusLabel->setText(QCoreApplication::translate("ScreenMainWork", "server offline", nullptr));
        ProfilePushButton->setText(QCoreApplication::translate("ScreenMainWork", "\320\237\321\200\320\276\321\204\320\270\320\273\321\214", nullptr));
        loginLabel->setText(QCoreApplication::translate("ScreenMainWork", "Yan2201", nullptr));
        nameLabel->setText(QCoreApplication::translate("ScreenMainWork", "Yan Batytskiy", nullptr));
        findLineEdit->setText(QString());
        mainWorkChatUserTabWidget->setTabText(mainWorkChatUserTabWidget->indexOf(mainWorkTabChatsList), QCoreApplication::translate("ScreenMainWork", "\320\247\320\260\321\202\321\213", nullptr));
        addressBookLabel->setText(QCoreApplication::translate("ScreenMainWork", "TextLabel", nullptr));
        addUserToChatPushButton->setText(QCoreApplication::translate("ScreenMainWork", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\277\320\276\320\273\321\203\321\207\320\260\321\202\320\265\320\273\321\217", nullptr));
        createNewChatPushButton->setText(QCoreApplication::translate("ScreenMainWork", "\321\201\320\276\320\267\320\264\320\260\321\202\321\214 \321\207\320\260\321\202", nullptr));
        mainWorkChatUserTabWidget->setTabText(mainWorkChatUserTabWidget->indexOf(mainWorkTabUsersList), QCoreApplication::translate("ScreenMainWork", "\320\237\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\320\270", nullptr));
        logOutPushButton->setText(QCoreApplication::translate("ScreenMainWork", "Log Out", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenMainWork: public Ui_ScreenMainWork {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_MAIN_WORK_H
