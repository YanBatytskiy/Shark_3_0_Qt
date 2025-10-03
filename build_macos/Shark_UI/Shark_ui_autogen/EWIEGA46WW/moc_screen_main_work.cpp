/****************************************************************************
** Meta object code from reading C++ file 'screen_main_work.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/screen_main_work.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screen_main_work.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN14ScreenMainWorkE_t {};
} // unnamed namespace

template <> constexpr inline auto ScreenMainWork::qt_create_metaobjectdata<qt_meta_tag_ZN14ScreenMainWorkE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ScreenMainWork",
        "signalStartNewChat",
        "",
        "turnOn",
        "signalAddContactToNewChat",
        "UserListModel*",
        "newChatUserListModel",
        "value",
        "signalClearUserDataToLabels",
        "signalMainWorkTransferrNewChatToMainChatList",
        "signalLogOut",
        "signalShowProfile",
        "slotConnectUserListToChatListchange",
        "status",
        "onConnectionStatusChanged",
        "connectionStatus",
        "ServerConnectionMode",
        "mode",
        "slotAddUserToNewChatList",
        "slotCancelNewChat",
        "slotMakeNewChat",
        "quantity",
        "const QStringListModel*",
        "participantsListModel",
        "slotNewChatUserListBecameEnabled",
        "slotMainWorkTransferrNewChatToMainChatList",
        "slotFindContactsByPart",
        "on_mainWorkChatUserTabWidget_currentChanged",
        "index",
        "on_createNewChatPushButton_clicked",
        "on_addUserToChatPushButton_clicked",
        "on_findLineEdit_textChanged",
        "arg1",
        "on_findLineEdit_editingFinished",
        "on_mainWorkUsersList_doubleClicked",
        "QModelIndex",
        "on_logOutPushButton_clicked",
        "on_ProfilePushButton_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalStartNewChat'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'signalAddContactToNewChat'
        QtMocHelpers::SignalData<void(UserListModel *, const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 5, 6 }, { QMetaType::QString, 7 },
        }}),
        // Signal 'signalClearUserDataToLabels'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signalMainWorkTransferrNewChatToMainChatList'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signalLogOut'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signalShowProfile'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotConnectUserListToChatListchange'
        QtMocHelpers::SlotData<void(bool)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 13 },
        }}),
        // Slot 'onConnectionStatusChanged'
        QtMocHelpers::SlotData<void(bool, ServerConnectionMode)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 15 }, { 0x80000000 | 16, 17 },
        }}),
        // Slot 'slotAddUserToNewChatList'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotCancelNewChat'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotMakeNewChat'
        QtMocHelpers::SlotData<void(int, const QStringListModel *)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 21 }, { 0x80000000 | 22, 23 },
        }}),
        // Slot 'slotNewChatUserListBecameEnabled'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotMainWorkTransferrNewChatToMainChatList'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotFindContactsByPart'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_mainWorkChatUserTabWidget_currentChanged'
        QtMocHelpers::SlotData<void(int)>(27, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 28 },
        }}),
        // Slot 'on_createNewChatPushButton_clicked'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_addUserToChatPushButton_clicked'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_findLineEdit_textChanged'
        QtMocHelpers::SlotData<void(const QString &)>(31, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 32 },
        }}),
        // Slot 'on_findLineEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_mainWorkUsersList_doubleClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(34, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 35, 28 },
        }}),
        // Slot 'on_logOutPushButton_clicked'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_ProfilePushButton_clicked'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ScreenMainWork, qt_meta_tag_ZN14ScreenMainWorkE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ScreenMainWork::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenMainWorkE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenMainWorkE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14ScreenMainWorkE_t>.metaTypes,
    nullptr
} };

void ScreenMainWork::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScreenMainWork *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalStartNewChat((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->signalAddContactToNewChat((*reinterpret_cast< std::add_pointer_t<UserListModel*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->signalClearUserDataToLabels(); break;
        case 3: _t->signalMainWorkTransferrNewChatToMainChatList(); break;
        case 4: _t->signalLogOut(); break;
        case 5: _t->signalShowProfile(); break;
        case 6: _t->slotConnectUserListToChatListchange((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->onConnectionStatusChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ServerConnectionMode>>(_a[2]))); break;
        case 8: _t->slotAddUserToNewChatList(); break;
        case 9: _t->slotCancelNewChat(); break;
        case 10: _t->slotMakeNewChat((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<const QStringListModel*>>(_a[2]))); break;
        case 11: _t->slotNewChatUserListBecameEnabled(); break;
        case 12: _t->slotMainWorkTransferrNewChatToMainChatList(); break;
        case 13: _t->slotFindContactsByPart(); break;
        case 14: _t->on_mainWorkChatUserTabWidget_currentChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->on_createNewChatPushButton_clicked(); break;
        case 16: _t->on_addUserToChatPushButton_clicked(); break;
        case 17: _t->on_findLineEdit_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->on_findLineEdit_editingFinished(); break;
        case 19: _t->on_mainWorkUsersList_doubleClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 20: _t->on_logOutPushButton_clicked(); break;
        case 21: _t->on_ProfilePushButton_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< UserListModel* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ScreenMainWork::*)(bool )>(_a, &ScreenMainWork::signalStartNewChat, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenMainWork::*)(UserListModel * , const QString & )>(_a, &ScreenMainWork::signalAddContactToNewChat, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenMainWork::*)()>(_a, &ScreenMainWork::signalClearUserDataToLabels, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenMainWork::*)()>(_a, &ScreenMainWork::signalMainWorkTransferrNewChatToMainChatList, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenMainWork::*)()>(_a, &ScreenMainWork::signalLogOut, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenMainWork::*)()>(_a, &ScreenMainWork::signalShowProfile, 5))
            return;
    }
}

const QMetaObject *ScreenMainWork::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenMainWork::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenMainWorkE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScreenMainWork::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void ScreenMainWork::signalStartNewChat(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ScreenMainWork::signalAddContactToNewChat(UserListModel * _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void ScreenMainWork::signalClearUserDataToLabels()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ScreenMainWork::signalMainWorkTransferrNewChatToMainChatList()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ScreenMainWork::signalLogOut()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ScreenMainWork::signalShowProfile()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
