/********************************************************************************
** Form generated from reading UI file 'screen_user_profile.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_USER_PROFILE_H
#define UI_SCREEN_USER_PROFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
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

class Ui_ScreenUserProfile
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *changePasswordLabel_2;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *loginLineEdit;
    QLabel *label_2;
    QLineEdit *nameLineEdit;
    QLabel *label_4;
    QLineEdit *emailLineEdit;
    QLabel *label_3;
    QLineEdit *phoneLineEdit;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *savePushButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelPushButton;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_6;
    QLabel *hintLabel;
    QSpacerItem *verticalSpacer_5;
    QLabel *changePasswordLabel;
    QFormLayout *formLayout_2;
    QLabel *newPasswordLabel;
    QLineEdit *passwordLineEdit;
    QLabel *confirmPasswordLabel;
    QLineEdit *confirnPasswordLineEdit;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *changePasswordPushButton;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ScreenUserProfile)
    {
        if (ScreenUserProfile->objectName().isEmpty())
            ScreenUserProfile->setObjectName("ScreenUserProfile");
        ScreenUserProfile->resize(1490, 1345);
        ScreenUserProfile->setMinimumSize(QSize(320, 0));
        gridLayout = new QGridLayout(ScreenUserProfile);
        gridLayout->setObjectName("gridLayout");
        verticalSpacer_4 = new QSpacerItem(20, 157, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(335, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 0, 1, 1);

        frame = new QFrame(ScreenUserProfile);
        frame->setObjectName("frame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(400, 0));
        frame->setMaximumSize(QSize(400, 16777215));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName("verticalLayout");
        changePasswordLabel_2 = new QLabel(frame);
        changePasswordLabel_2->setObjectName("changePasswordLabel_2");
        QFont font;
        font.setBold(true);
        changePasswordLabel_2->setFont(font);
        changePasswordLabel_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(changePasswordLabel_2);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(frame);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label);

        loginLineEdit = new QLineEdit(frame);
        loginLineEdit->setObjectName("loginLineEdit");
        loginLineEdit->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(loginLineEdit->sizePolicy().hasHeightForWidth());
        loginLineEdit->setSizePolicy(sizePolicy1);
        loginLineEdit->setMinimumSize(QSize(250, 0));
        loginLineEdit->setMaximumSize(QSize(250, 16777215));
        loginLineEdit->setStyleSheet(QString::fromUtf8("color: black; border:1px solid #aaa; border-radius:4px; padding:2px;"));
        loginLineEdit->setReadOnly(true);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, loginLineEdit);

        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_2);

        nameLineEdit = new QLineEdit(frame);
        nameLineEdit->setObjectName("nameLineEdit");
        nameLineEdit->setEnabled(true);
        sizePolicy1.setHeightForWidth(nameLineEdit->sizePolicy().hasHeightForWidth());
        nameLineEdit->setSizePolicy(sizePolicy1);
        nameLineEdit->setMinimumSize(QSize(250, 0));
        nameLineEdit->setMaximumSize(QSize(250, 16777215));
        nameLineEdit->setStyleSheet(QString::fromUtf8("color: black; border:1px solid #aaa; border-radius:4px; padding:2px;"));

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, nameLineEdit);

        label_4 = new QLabel(frame);
        label_4->setObjectName("label_4");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_4);

        emailLineEdit = new QLineEdit(frame);
        emailLineEdit->setObjectName("emailLineEdit");
        emailLineEdit->setEnabled(true);
        sizePolicy1.setHeightForWidth(emailLineEdit->sizePolicy().hasHeightForWidth());
        emailLineEdit->setSizePolicy(sizePolicy1);
        emailLineEdit->setMinimumSize(QSize(250, 0));
        emailLineEdit->setMaximumSize(QSize(250, 16777215));
        emailLineEdit->setStyleSheet(QString::fromUtf8("color: black; border:1px solid #aaa; border-radius:4px; padding:2px;"));

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, emailLineEdit);

        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_3);

        phoneLineEdit = new QLineEdit(frame);
        phoneLineEdit->setObjectName("phoneLineEdit");
        phoneLineEdit->setEnabled(true);
        sizePolicy1.setHeightForWidth(phoneLineEdit->sizePolicy().hasHeightForWidth());
        phoneLineEdit->setSizePolicy(sizePolicy1);
        phoneLineEdit->setMinimumSize(QSize(250, 0));
        phoneLineEdit->setMaximumSize(QSize(250, 16777215));
        phoneLineEdit->setStyleSheet(QString::fromUtf8("color: black; border:1px solid #aaa; border-radius:4px; padding:2px;"));

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, phoneLineEdit);


        verticalLayout->addLayout(formLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        savePushButton = new QPushButton(frame);
        savePushButton->setObjectName("savePushButton");
        savePushButton->setEnabled(true);

        horizontalLayout->addWidget(savePushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelPushButton = new QPushButton(frame);
        cancelPushButton->setObjectName("cancelPushButton");
        cancelPushButton->setEnabled(true);
        cancelPushButton->setStyleSheet(QString::fromUtf8("color: rgb(224, 27, 36);"));

        horizontalLayout->addWidget(cancelPushButton);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_6 = new QSpacerItem(20, 13, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_6);

        hintLabel = new QLabel(frame);
        hintLabel->setObjectName("hintLabel");
        sizePolicy1.setHeightForWidth(hintLabel->sizePolicy().hasHeightForWidth());
        hintLabel->setSizePolicy(sizePolicy1);
        hintLabel->setMinimumSize(QSize(300, 32));
        hintLabel->setMaximumSize(QSize(300, 32));
        hintLabel->setWordWrap(true);

        verticalLayout->addWidget(hintLabel);

        verticalSpacer_5 = new QSpacerItem(40, 13, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_5);

        changePasswordLabel = new QLabel(frame);
        changePasswordLabel->setObjectName("changePasswordLabel");
        changePasswordLabel->setFont(font);
        changePasswordLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(changePasswordLabel);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName("formLayout_2");
        newPasswordLabel = new QLabel(frame);
        newPasswordLabel->setObjectName("newPasswordLabel");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::LabelRole, newPasswordLabel);

        passwordLineEdit = new QLineEdit(frame);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setEnabled(true);
        sizePolicy1.setHeightForWidth(passwordLineEdit->sizePolicy().hasHeightForWidth());
        passwordLineEdit->setSizePolicy(sizePolicy1);
        passwordLineEdit->setMinimumSize(QSize(250, 0));
        passwordLineEdit->setMaximumSize(QSize(250, 16777215));
        passwordLineEdit->setStyleSheet(QString::fromUtf8("color: black; border:1px solid #aaa; border-radius:4px; padding:2px;"));
        passwordLineEdit->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit);

        formLayout_2->setWidget(0, QFormLayout::ItemRole::FieldRole, passwordLineEdit);

        confirmPasswordLabel = new QLabel(frame);
        confirmPasswordLabel->setObjectName("confirmPasswordLabel");

        formLayout_2->setWidget(1, QFormLayout::ItemRole::LabelRole, confirmPasswordLabel);

        confirnPasswordLineEdit = new QLineEdit(frame);
        confirnPasswordLineEdit->setObjectName("confirnPasswordLineEdit");
        confirnPasswordLineEdit->setEnabled(true);
        sizePolicy1.setHeightForWidth(confirnPasswordLineEdit->sizePolicy().hasHeightForWidth());
        confirnPasswordLineEdit->setSizePolicy(sizePolicy1);
        confirnPasswordLineEdit->setMinimumSize(QSize(250, 0));
        confirnPasswordLineEdit->setMaximumSize(QSize(250, 16777215));
        confirnPasswordLineEdit->setStyleSheet(QString::fromUtf8("color: black; border:1px solid #aaa; border-radius:4px; padding:2px;"));
        confirnPasswordLineEdit->setEchoMode(QLineEdit::EchoMode::PasswordEchoOnEdit);

        formLayout_2->setWidget(1, QFormLayout::ItemRole::FieldRole, confirnPasswordLineEdit);


        verticalLayout->addLayout(formLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        changePasswordPushButton = new QPushButton(frame);
        changePasswordPushButton->setObjectName("changePasswordPushButton");

        horizontalLayout_3->addWidget(changePasswordPushButton);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        gridLayout->addWidget(frame, 1, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(335, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 156, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 1, 1, 1);


        retranslateUi(ScreenUserProfile);

        QMetaObject::connectSlotsByName(ScreenUserProfile);
    } // setupUi

    void retranslateUi(QWidget *ScreenUserProfile)
    {
        ScreenUserProfile->setWindowTitle(QCoreApplication::translate("ScreenUserProfile", "Form", nullptr));
        changePasswordLabel_2->setText(QCoreApplication::translate("ScreenUserProfile", "\320\224\320\260\320\275\320\275\321\213\320\265 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\217", nullptr));
        label->setText(QCoreApplication::translate("ScreenUserProfile", "\320\233\320\276\320\263\320\270\320\275:", nullptr));
        label_2->setText(QCoreApplication::translate("ScreenUserProfile", "\320\230\320\274\321\217:", nullptr));
        label_4->setText(QCoreApplication::translate("ScreenUserProfile", "Email:", nullptr));
#if QT_CONFIG(tooltip)
        emailLineEdit->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        label_3->setText(QCoreApplication::translate("ScreenUserProfile", "\320\242\320\265\320\273\320\265\321\204\320\276\320\275:", nullptr));
        savePushButton->setText(QCoreApplication::translate("ScreenUserProfile", "\320\227\320\260\320\277\320\270\321\201\320\260\321\202\321\214", nullptr));
        cancelPushButton->setText(QCoreApplication::translate("ScreenUserProfile", "\320\222\321\213\320\271\321\202\320\270", nullptr));
        hintLabel->setText(QString());
        changePasswordLabel->setText(QCoreApplication::translate("ScreenUserProfile", "\320\241\320\274\320\265\320\275\320\260 \320\277\320\260\321\200\320\276\320\273\321\217", nullptr));
        newPasswordLabel->setText(QCoreApplication::translate("ScreenUserProfile", "\320\235\320\276\320\262\321\213\320\271 \320\277\320\260\321\200\320\276\320\273\321\214:", nullptr));
#if QT_CONFIG(tooltip)
        passwordLineEdit->setToolTip(QCoreApplication::translate("ScreenUserProfile", "\320\235\320\265 \320\274\320\265\320\275\320\265\320\265 5 \320\270 \320\275\320\265 \320\261\320\276\320\273\320\265\320\265 20 \321\201\320\270\320\274\320\262\320\276\320\273\320\276\320\262, \320\234\320\275\320\270\320\274\321\203\320\274 \320\276\320\264\320\275\320\260 \320\267\320\260\320\263\320\273\320\260\320\262\320\275\320\260\321\217 \320\270 \320\276\320\264\320\275\320\260 \321\206\320\270\321\204\321\200\320\260 (\321\202\320\276\320\273\321\214\320\272\320\276 \320\273\320\260\321\202\320\270\320\275\321\201\320\272\320\270\320\265 \320\261\321\203\320\272\320\262\321\213)", nullptr));
#endif // QT_CONFIG(tooltip)
        confirmPasswordLabel->setText(QCoreApplication::translate("ScreenUserProfile", "\320\237\320\276\320\262\321\202\320\276\321\200 \320\277\320\260\321\200\320\276\320\273\321\217:", nullptr));
#if QT_CONFIG(tooltip)
        confirnPasswordLineEdit->setToolTip(QCoreApplication::translate("ScreenUserProfile", "\320\237\320\276\320\262\321\202\320\276\321\200\320\270\321\202\320\265 \320\262\320\262\320\276\320\264 \320\277\320\260\321\200\320\276\320\273\321\217", nullptr));
#endif // QT_CONFIG(tooltip)
        changePasswordPushButton->setText(QCoreApplication::translate("ScreenUserProfile", "\320\241\320\274\320\265\320\275\320\270\321\202\321\214 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScreenUserProfile: public Ui_ScreenUserProfile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_USER_PROFILE_H
