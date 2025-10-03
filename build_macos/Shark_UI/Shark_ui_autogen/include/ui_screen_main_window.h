/********************************************************************************
** Form generated from reading UI file 'screen_main_window.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_MAIN_WINDOW_H
#define UI_SCREEN_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "screen_login.h"
#include "screen_main_work.h"
#include "screen_register.h"
#include "screen_user_profile.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *userProfileAction;
    QAction *exitAction;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *mainWindowstackedWidget;
    ScreenLogin *pageLogin;
    ScreenRegister *pageRegister;
    ScreenMainWork *pageWork;
    ScreenUserProfile *pageProfile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1199, 851);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        userProfileAction = new QAction(MainWindow);
        userProfileAction->setObjectName("userProfileAction");
        exitAction = new QAction(MainWindow);
        exitAction->setObjectName("exitAction");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        mainWindowstackedWidget = new QStackedWidget(centralwidget);
        mainWindowstackedWidget->setObjectName("mainWindowstackedWidget");
        sizePolicy.setHeightForWidth(mainWindowstackedWidget->sizePolicy().hasHeightForWidth());
        mainWindowstackedWidget->setSizePolicy(sizePolicy);
        pageLogin = new ScreenLogin();
        pageLogin->setObjectName("pageLogin");
        mainWindowstackedWidget->addWidget(pageLogin);
        pageRegister = new ScreenRegister();
        pageRegister->setObjectName("pageRegister");
        mainWindowstackedWidget->addWidget(pageRegister);
        pageWork = new ScreenMainWork();
        pageWork->setObjectName("pageWork");
        mainWindowstackedWidget->addWidget(pageWork);
        pageProfile = new ScreenUserProfile();
        pageProfile->setObjectName("pageProfile");
        mainWindowstackedWidget->addWidget(pageProfile);

        verticalLayout->addWidget(mainWindowstackedWidget);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        mainWindowstackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Shark - 3.0Qt", nullptr));
        userProfileAction->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\276\321\204\320\270\320\273\321\214 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217", nullptr));
        exitAction->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\321\205\320\276\320\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_MAIN_WINDOW_H
