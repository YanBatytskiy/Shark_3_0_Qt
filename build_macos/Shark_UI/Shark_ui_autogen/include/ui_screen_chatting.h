/********************************************************************************
** Form generated from reading UI file 'screen_chatting.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_CHATTING_H
#define UI_SCREEN_CHATTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScreenChatting
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QListView *ScreenChattingMessagesList;
    QHBoxLayout *horizontalLayout;
    QTextEdit *ScreenChattingNewMessageTextEdit;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *ScreenChattingSendMessagePushButton;

    void setupUi(QWidget *ScreenChatting)
    {
        if (ScreenChatting->objectName().isEmpty())
            ScreenChatting->setObjectName("ScreenChatting");
        ScreenChatting->resize(1303, 920);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScreenChatting->sizePolicy().hasHeightForWidth());
        ScreenChatting->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(ScreenChatting);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        ScreenChattingMessagesList = new QListView(ScreenChatting);
        ScreenChattingMessagesList->setObjectName("ScreenChattingMessagesList");
        sizePolicy.setHeightForWidth(ScreenChattingMessagesList->sizePolicy().hasHeightForWidth());
        ScreenChattingMessagesList->setSizePolicy(sizePolicy);
        ScreenChattingMessagesList->setStyleSheet(QString::fromUtf8("border-image: url(:/images/sea_1.jpg) 0 0 0 0 stretch stretch;"));
        ScreenChattingMessagesList->setVerticalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
        ScreenChattingMessagesList->setHorizontalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);

        verticalLayout_2->addWidget(ScreenChattingMessagesList);

        verticalLayout_2->setStretch(0, 1);

        verticalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        ScreenChattingNewMessageTextEdit = new QTextEdit(ScreenChatting);
        ScreenChattingNewMessageTextEdit->setObjectName("ScreenChattingNewMessageTextEdit");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ScreenChattingNewMessageTextEdit->sizePolicy().hasHeightForWidth());
        ScreenChattingNewMessageTextEdit->setSizePolicy(sizePolicy1);
        ScreenChattingNewMessageTextEdit->setMinimumSize(QSize(0, 90));
        ScreenChattingNewMessageTextEdit->setMaximumSize(QSize(16777215, 90));
        ScreenChattingNewMessageTextEdit->setLineWidth(4);

        horizontalLayout->addWidget(ScreenChattingNewMessageTextEdit);

        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 13, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        ScreenChattingSendMessagePushButton = new QPushButton(ScreenChatting);
        ScreenChattingSendMessagePushButton->setObjectName("ScreenChattingSendMessagePushButton");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ScreenChattingSendMessagePushButton->sizePolicy().hasHeightForWidth());
        ScreenChattingSendMessagePushButton->setSizePolicy(sizePolicy2);
        ScreenChattingSendMessagePushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(153, 193, 241)"));

        verticalLayout->addWidget(ScreenChattingSendMessagePushButton);


        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 1);

        verticalLayout_3->addLayout(horizontalLayout);

        verticalLayout_3->setStretch(0, 1);

        retranslateUi(ScreenChatting);

        QMetaObject::connectSlotsByName(ScreenChatting);
    } // setupUi

    void retranslateUi(QWidget *ScreenChatting)
    {
        ScreenChatting->setWindowTitle(QCoreApplication::translate("ScreenChatting", "Form", nullptr));
        ScreenChattingNewMessageTextEdit->setPlaceholderText(QCoreApplication::translate("ScreenChatting", "\320\262\320\262\320\265\320\264\320\270\321\202\320\265 \321\201\320\276\320\276\320\261\321\211\320\265\320\275\320\270\320\265...", nullptr));
        ScreenChattingSendMessagePushButton->setText(QCoreApplication::translate("ScreenChatting", "\320\236\321\202\320\277\321\200\320\260\320\262\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenChatting: public Ui_ScreenChatting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_CHATTING_H
