/********************************************************************************
** Form generated from reading UI file 'screen_chat_list.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_CHAT_LIST_H
#define UI_SCREEN_CHAT_LIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenChatList
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListView *chatListView;

    void setupUi(QWidget *ScreenChatList)
    {
        if (ScreenChatList->objectName().isEmpty())
            ScreenChatList->setObjectName("ScreenChatList");
        ScreenChatList->resize(1041, 795);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScreenChatList->sizePolicy().hasHeightForWidth());
        ScreenChatList->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(ScreenChatList);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(ScreenChatList);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        chatListView = new QListView(ScreenChatList);
        chatListView->setObjectName("chatListView");
        sizePolicy.setHeightForWidth(chatListView->sizePolicy().hasHeightForWidth());
        chatListView->setSizePolicy(sizePolicy);
        chatListView->setMouseTracking(true);
        chatListView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
        chatListView->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
        chatListView->setHorizontalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);

        verticalLayout->addWidget(chatListView);


        retranslateUi(ScreenChatList);

        QMetaObject::connectSlotsByName(ScreenChatList);
    } // setupUi

    void retranslateUi(QWidget *ScreenChatList)
    {
        ScreenChatList->setWindowTitle(QCoreApplication::translate("ScreenChatList", "Form", nullptr));
        label->setText(QCoreApplication::translate("ScreenChatList", "\320\241\320\277\320\270\321\201\320\276\320\272 \321\207\320\260\321\202\320\276\320\262:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenChatList: public Ui_ScreenChatList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_CHAT_LIST_H
