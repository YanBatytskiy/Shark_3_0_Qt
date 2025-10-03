/****************************************************************************
** Meta object code from reading C++ file 'screen_user_data.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/screen_user_data.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screen_user_data.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14ScreenUserDataE_t {};
} // unnamed namespace

template <> constexpr inline auto ScreenUserData::qt_create_metaobjectdata<qt_meta_tag_ZN14ScreenUserDataE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ScreenUserData",
        "signalNewChatListBecameEnabled",
        "",
        "signal_on_block_push_button_clicked",
        "signal_on_unblock_push_button_clicked",
        "signal_on_bun_push_button_clicked",
        "signal_on_unbun_push_button_clicked",
        "slot_on_block_push_button_clicked",
        "slot_on_unblock_push_button_clicked",
        "slot_on_bun_push_button_clicked",
        "slot_on_unbun_push_button_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalNewChatListBecameEnabled'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signal_on_block_push_button_clicked'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signal_on_unblock_push_button_clicked'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signal_on_bun_push_button_clicked'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signal_on_unbun_push_button_clicked'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slot_on_block_push_button_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_on_unblock_push_button_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_on_bun_push_button_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_on_unbun_push_button_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ScreenUserData, qt_meta_tag_ZN14ScreenUserDataE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ScreenUserData::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenUserDataE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenUserDataE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14ScreenUserDataE_t>.metaTypes,
    nullptr
} };

void ScreenUserData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScreenUserData *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalNewChatListBecameEnabled(); break;
        case 1: _t->signal_on_block_push_button_clicked(); break;
        case 2: _t->signal_on_unblock_push_button_clicked(); break;
        case 3: _t->signal_on_bun_push_button_clicked(); break;
        case 4: _t->signal_on_unbun_push_button_clicked(); break;
        case 5: _t->slot_on_block_push_button_clicked(); break;
        case 6: _t->slot_on_unblock_push_button_clicked(); break;
        case 7: _t->slot_on_bun_push_button_clicked(); break;
        case 8: _t->slot_on_unbun_push_button_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ScreenUserData::*)()>(_a, &ScreenUserData::signalNewChatListBecameEnabled, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenUserData::*)()>(_a, &ScreenUserData::signal_on_block_push_button_clicked, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenUserData::*)()>(_a, &ScreenUserData::signal_on_unblock_push_button_clicked, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenUserData::*)()>(_a, &ScreenUserData::signal_on_bun_push_button_clicked, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenUserData::*)()>(_a, &ScreenUserData::signal_on_unbun_push_button_clicked, 4))
            return;
    }
}

const QMetaObject *ScreenUserData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenUserData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenUserDataE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScreenUserData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ScreenUserData::signalNewChatListBecameEnabled()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ScreenUserData::signal_on_block_push_button_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ScreenUserData::signal_on_unblock_push_button_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ScreenUserData::signal_on_bun_push_button_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ScreenUserData::signal_on_unbun_push_button_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
