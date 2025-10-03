/****************************************************************************
** Meta object code from reading C++ file 'screen_new_chat_participants.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/screen_new_chat_participants.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screen_new_chat_participants.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN25ScreenNewChatParticipantsE_t {};
} // unnamed namespace

template <> constexpr inline auto ScreenNewChatParticipants::qt_create_metaobjectdata<qt_meta_tag_ZN25ScreenNewChatParticipantsE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ScreenNewChatParticipants",
        "signalCancelNewChat",
        "",
        "signalMakeNewChat",
        "quantity",
        "const QStringListModel*",
        "participantsListModel",
        "signalNewChatUserListBecameEnabled",
        "slotCollectParticipantsForNewChat",
        "turnOn",
        "slotAddContactToParticipantsList",
        "UserListModel*",
        "newChatUserListModel",
        "value",
        "slotScreenNewChatTransferrNewChatToMainChatList",
        "on_screenNewChatDeleteContactPushButton_clicked",
        "on_screenUserDataCancelNewChatPushButton_clicked",
        "on_screenUserDataCreateNewChatPushButton_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalCancelNewChat'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signalMakeNewChat'
        QtMocHelpers::SignalData<void(int, const QStringListModel *)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 }, { 0x80000000 | 5, 6 },
        }}),
        // Signal 'signalNewChatUserListBecameEnabled'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotCollectParticipantsForNewChat'
        QtMocHelpers::SlotData<void(bool)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 9 },
        }}),
        // Slot 'slotAddContactToParticipantsList'
        QtMocHelpers::SlotData<void(UserListModel *, const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 }, { QMetaType::QString, 13 },
        }}),
        // Slot 'slotScreenNewChatTransferrNewChatToMainChatList'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_screenNewChatDeleteContactPushButton_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_screenUserDataCancelNewChatPushButton_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_screenUserDataCreateNewChatPushButton_clicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ScreenNewChatParticipants, qt_meta_tag_ZN25ScreenNewChatParticipantsE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ScreenNewChatParticipants::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN25ScreenNewChatParticipantsE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN25ScreenNewChatParticipantsE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN25ScreenNewChatParticipantsE_t>.metaTypes,
    nullptr
} };

void ScreenNewChatParticipants::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScreenNewChatParticipants *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalCancelNewChat(); break;
        case 1: _t->signalMakeNewChat((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<const QStringListModel*>>(_a[2]))); break;
        case 2: _t->signalNewChatUserListBecameEnabled(); break;
        case 3: _t->slotCollectParticipantsForNewChat((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->slotAddContactToParticipantsList((*reinterpret_cast< std::add_pointer_t<UserListModel*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->slotScreenNewChatTransferrNewChatToMainChatList(); break;
        case 6: _t->on_screenNewChatDeleteContactPushButton_clicked(); break;
        case 7: _t->on_screenUserDataCancelNewChatPushButton_clicked(); break;
        case 8: _t->on_screenUserDataCreateNewChatPushButton_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< UserListModel* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ScreenNewChatParticipants::*)()>(_a, &ScreenNewChatParticipants::signalCancelNewChat, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenNewChatParticipants::*)(int , const QStringListModel * )>(_a, &ScreenNewChatParticipants::signalMakeNewChat, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenNewChatParticipants::*)()>(_a, &ScreenNewChatParticipants::signalNewChatUserListBecameEnabled, 2))
            return;
    }
}

const QMetaObject *ScreenNewChatParticipants::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenNewChatParticipants::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN25ScreenNewChatParticipantsE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScreenNewChatParticipants::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ScreenNewChatParticipants::signalCancelNewChat()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ScreenNewChatParticipants::signalMakeNewChat(int _t1, const QStringListModel * _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void ScreenNewChatParticipants::signalNewChatUserListBecameEnabled()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
