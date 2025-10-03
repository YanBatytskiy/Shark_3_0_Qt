/****************************************************************************
** Meta object code from reading C++ file 'screen_chat_list.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/screen_chat_list.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screen_chat_list.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14ScreenChatListE_t {};
} // unnamed namespace

template <> constexpr inline auto ScreenChatList::qt_create_metaobjectdata<qt_meta_tag_ZN14ScreenChatListE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ScreenChatList",
        "signalCurrentChatIndexChanged",
        "",
        "QModelIndex",
        "current",
        "previous",
        "signalUserListIdChanged",
        "std::string",
        "login",
        "slotOnUserListIndexChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalCurrentChatIndexChanged'
        QtMocHelpers::SignalData<void(const QModelIndex &, const QModelIndex &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 3, 5 },
        }}),
        // Signal 'signalUserListIdChanged'
        QtMocHelpers::SignalData<void(std::string)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'slotOnUserListIndexChanged'
        QtMocHelpers::SlotData<void(const QModelIndex &, const QModelIndex &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 3, 5 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ScreenChatList, qt_meta_tag_ZN14ScreenChatListE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ScreenChatList::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenChatListE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenChatListE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14ScreenChatListE_t>.metaTypes,
    nullptr
} };

void ScreenChatList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScreenChatList *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalCurrentChatIndexChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        case 1: _t->signalUserListIdChanged((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 2: _t->slotOnUserListIndexChanged((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ScreenChatList::*)(const QModelIndex & , const QModelIndex & )>(_a, &ScreenChatList::signalCurrentChatIndexChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenChatList::*)(std::string )>(_a, &ScreenChatList::signalUserListIdChanged, 1))
            return;
    }
}

const QMetaObject *ScreenChatList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenChatList::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenChatListE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScreenChatList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ScreenChatList::signalCurrentChatIndexChanged(const QModelIndex & _t1, const QModelIndex & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void ScreenChatList::signalUserListIdChanged(std::string _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
