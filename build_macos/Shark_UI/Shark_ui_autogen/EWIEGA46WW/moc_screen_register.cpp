/****************************************************************************
** Meta object code from reading C++ file 'screen_register.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/screen_register.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screen_register.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14ScreenRegisterE_t {};
} // unnamed namespace

template <> constexpr inline auto ScreenRegister::qt_create_metaobjectdata<qt_meta_tag_ZN14ScreenRegisterE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ScreenRegister",
        "loginRequested",
        "",
        "rejected",
        "signalLoggedIn",
        "login",
        "onConnectionStatusChanged",
        "connectionStatus",
        "ServerConnectionMode",
        "mode",
        "on_toLoginButton_clicked",
        "on_loginEdit_editingFinished",
        "on_nameEdit_editingFinished",
        "on_passwordEdit_editingFinished",
        "on_passwordConfirmEdit_editingFinished",
        "on_exitPushButton_clicked",
        "on_registerPushButton_clicked",
        "on_passwordConfirmEdit_cursorPositionChanged",
        "arg1",
        "arg2"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'loginRequested'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'rejected'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signalLoggedIn'
        QtMocHelpers::SignalData<void(QString)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Slot 'onConnectionStatusChanged'
        QtMocHelpers::SlotData<void(bool, ServerConnectionMode)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 7 }, { 0x80000000 | 8, 9 },
        }}),
        // Slot 'on_toLoginButton_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_loginEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_nameEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_passwordEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_passwordConfirmEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_exitPushButton_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_registerPushButton_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_passwordConfirmEdit_cursorPositionChanged'
        QtMocHelpers::SlotData<void(int, int)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Int, 19 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ScreenRegister, qt_meta_tag_ZN14ScreenRegisterE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ScreenRegister::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenRegisterE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenRegisterE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14ScreenRegisterE_t>.metaTypes,
    nullptr
} };

void ScreenRegister::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScreenRegister *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->loginRequested(); break;
        case 1: _t->rejected(); break;
        case 2: _t->signalLoggedIn((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->onConnectionStatusChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<ServerConnectionMode>>(_a[2]))); break;
        case 4: _t->on_toLoginButton_clicked(); break;
        case 5: _t->on_loginEdit_editingFinished(); break;
        case 6: _t->on_nameEdit_editingFinished(); break;
        case 7: _t->on_passwordEdit_editingFinished(); break;
        case 8: _t->on_passwordConfirmEdit_editingFinished(); break;
        case 9: _t->on_exitPushButton_clicked(); break;
        case 10: _t->on_registerPushButton_clicked(); break;
        case 11: _t->on_passwordConfirmEdit_cursorPositionChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ScreenRegister::*)()>(_a, &ScreenRegister::loginRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenRegister::*)()>(_a, &ScreenRegister::rejected, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenRegister::*)(QString )>(_a, &ScreenRegister::signalLoggedIn, 2))
            return;
    }
}

const QMetaObject *ScreenRegister::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenRegister::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ScreenRegisterE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScreenRegister::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ScreenRegister::loginRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ScreenRegister::rejected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ScreenRegister::signalLoggedIn(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
