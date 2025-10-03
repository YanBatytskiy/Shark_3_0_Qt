/****************************************************************************
** Meta object code from reading C++ file 'screen_login.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/screen_login.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screen_login.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11ScreenLoginE_t {};
} // unnamed namespace

template <> constexpr inline auto ScreenLogin::qt_create_metaobjectdata<qt_meta_tag_ZN11ScreenLoginE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ScreenLogin",
        "signal_registration_requested",
        "",
        "accepted",
        "login",
        "rejected",
        "onConnectionStatusChanged",
        "connectionStatus",
        "ServerConnectionMode",
        "mode",
        "slot_show_logger_form",
        "std::multimap<qint64,QString>",
        "logger_model",
        "slot_on_register_mode_button_clicked",
        "on_loginButtonBox_accepted",
        "on_loginButtonBox_rejected",
        "checkSignIn",
        "on_loginEdit_returnPressed",
        "on_passwordEdit_returnPressed",
        "on_baseReInitialisationPushButton_clicked",
        "slotOn_logFileClearPushButton_clicked",
        "slotOn_lookLogSeveralLinePushButton_clicked",
        "slotOn_lookLogLastLinePushButton_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signal_registration_requested'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'accepted'
        QtMocHelpers::SignalData<void(QString)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Signal 'rejected'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onConnectionStatusChanged'
        QtMocHelpers::SlotData<void(bool, ServerConnectionMode)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 7 }, { 0x80000000 | 8, 9 },
        }}),
        // Slot 'slot_show_logger_form'
        QtMocHelpers::SlotData<void(const std::multimap<qint64,QString> &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'slot_on_register_mode_button_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_loginButtonBox_accepted'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_loginButtonBox_rejected'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'checkSignIn'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_loginEdit_returnPressed'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_passwordEdit_returnPressed'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_baseReInitialisationPushButton_clicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotOn_logFileClearPushButton_clicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotOn_lookLogSeveralLinePushButton_clicked'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slotOn_lookLogLastLinePushButton_clicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ScreenLogin, qt_meta_tag_ZN11ScreenLoginE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ScreenLogin::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ScreenLoginE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ScreenLoginE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11ScreenLoginE_t>.metaTypes,
    nullptr
} };

void ScreenLogin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScreenLogin *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signal_registration_requested(); break;
        case 1: _t->accepted((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->rejected(); break;
        case 3: _t->onConnectionStatusChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ServerConnectionMode>>(_a[2]))); break;
        case 4: _t->slot_show_logger_form((*reinterpret_cast< std::add_pointer_t<std::multimap<qint64,QString>>>(_a[1]))); break;
        case 5: _t->slot_on_register_mode_button_clicked(); break;
        case 6: _t->on_loginButtonBox_accepted(); break;
        case 7: _t->on_loginButtonBox_rejected(); break;
        case 8: _t->checkSignIn(); break;
        case 9: _t->on_loginEdit_returnPressed(); break;
        case 10: _t->on_passwordEdit_returnPressed(); break;
        case 11: _t->on_baseReInitialisationPushButton_clicked(); break;
        case 12: _t->slotOn_logFileClearPushButton_clicked(); break;
        case 13: _t->slotOn_lookLogSeveralLinePushButton_clicked(); break;
        case 14: _t->slotOn_lookLogLastLinePushButton_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ScreenLogin::*)()>(_a, &ScreenLogin::signal_registration_requested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenLogin::*)(QString )>(_a, &ScreenLogin::accepted, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenLogin::*)()>(_a, &ScreenLogin::rejected, 2))
            return;
    }
}

const QMetaObject *ScreenLogin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenLogin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ScreenLoginE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScreenLogin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void ScreenLogin::signal_registration_requested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ScreenLogin::accepted(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void ScreenLogin::rejected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
