/********************************************************************************
** Form generated from reading UI file 'screen_new_chat_participants.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_NEW_CHAT_PARTICIPANTS_H
#define UI_SCREEN_NEW_CHAT_PARTICIPANTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenNewChatParticipants
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *screenUserDataNewChatLabel;
    QSpacerItem *verticalSpacer_8;
    QListView *screenUserDataNewChatUsersList;
    QSpacerItem *verticalSpacer_13;
    QPushButton *screenNewChatDeleteContactPushButton;
    QSpacerItem *verticalSpacer_14;
    QHBoxLayout *horizontalLayout;
    QPushButton *screenUserDataCreateNewChatPushButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *screenUserDataCancelNewChatPushButton;

    void setupUi(QWidget *ScreenNewChatParticipants)
    {
        if (ScreenNewChatParticipants->objectName().isEmpty())
            ScreenNewChatParticipants->setObjectName("ScreenNewChatParticipants");
        ScreenNewChatParticipants->resize(361, 382);
        verticalLayout = new QVBoxLayout(ScreenNewChatParticipants);
        verticalLayout->setObjectName("verticalLayout");
        screenUserDataNewChatLabel = new QLabel(ScreenNewChatParticipants);
        screenUserDataNewChatLabel->setObjectName("screenUserDataNewChatLabel");
        screenUserDataNewChatLabel->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(screenUserDataNewChatLabel->sizePolicy().hasHeightForWidth());
        screenUserDataNewChatLabel->setSizePolicy(sizePolicy);
        QFont font;
        font.setBold(true);
        screenUserDataNewChatLabel->setFont(font);

        verticalLayout->addWidget(screenUserDataNewChatLabel);

        verticalSpacer_8 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_8);

        screenUserDataNewChatUsersList = new QListView(ScreenNewChatParticipants);
        screenUserDataNewChatUsersList->setObjectName("screenUserDataNewChatUsersList");
        screenUserDataNewChatUsersList->setEnabled(true);

        verticalLayout->addWidget(screenUserDataNewChatUsersList);

        verticalSpacer_13 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_13);

        screenNewChatDeleteContactPushButton = new QPushButton(ScreenNewChatParticipants);
        screenNewChatDeleteContactPushButton->setObjectName("screenNewChatDeleteContactPushButton");

        verticalLayout->addWidget(screenNewChatDeleteContactPushButton);

        verticalSpacer_14 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_14);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        screenUserDataCreateNewChatPushButton = new QPushButton(ScreenNewChatParticipants);
        screenUserDataCreateNewChatPushButton->setObjectName("screenUserDataCreateNewChatPushButton");
        screenUserDataCreateNewChatPushButton->setEnabled(true);
        screenUserDataCreateNewChatPushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 174);"));

        horizontalLayout->addWidget(screenUserDataCreateNewChatPushButton);

        horizontalSpacer = new QSpacerItem(40, 13, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        screenUserDataCancelNewChatPushButton = new QPushButton(ScreenNewChatParticipants);
        screenUserDataCancelNewChatPushButton->setObjectName("screenUserDataCancelNewChatPushButton");
        screenUserDataCancelNewChatPushButton->setEnabled(true);
        screenUserDataCancelNewChatPushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 180, 181);"));

        horizontalLayout->addWidget(screenUserDataCancelNewChatPushButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ScreenNewChatParticipants);

        QMetaObject::connectSlotsByName(ScreenNewChatParticipants);
    } // setupUi

    void retranslateUi(QWidget *ScreenNewChatParticipants)
    {
        ScreenNewChatParticipants->setWindowTitle(QCoreApplication::translate("ScreenNewChatParticipants", "Form", nullptr));
        screenUserDataNewChatLabel->setText(QCoreApplication::translate("ScreenNewChatParticipants", "\320\235\320\276\320\262\321\213\320\271 \321\207\320\260\321\202 - \321\203\321\207\320\260\321\201\321\202\320\275\320\270\320\272\320\270 (\320\273\320\276\320\263\320\270\320\275\321\213):", nullptr));
        screenNewChatDeleteContactPushButton->setText(QCoreApplication::translate("ScreenNewChatParticipants", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\272\320\276\320\275\321\202\320\260\320\272\321\202 \320\270\320\267 \321\201\320\277\320\270\321\201\320\272\320\260", nullptr));
        screenUserDataCreateNewChatPushButton->setText(QCoreApplication::translate("ScreenNewChatParticipants", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214 \321\201\320\276\320\276\320\261\321\211\320\265\320\275\320\270\320\265", nullptr));
        screenUserDataCancelNewChatPushButton->setText(QCoreApplication::translate("ScreenNewChatParticipants", "\320\236\321\202\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenNewChatParticipants: public Ui_ScreenNewChatParticipants {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_NEW_CHAT_PARTICIPANTS_H
