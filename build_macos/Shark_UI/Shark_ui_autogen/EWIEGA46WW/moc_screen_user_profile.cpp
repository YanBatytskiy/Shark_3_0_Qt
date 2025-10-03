/****************************************************************************
** Meta object code from reading C++ file 'screen_user_profile.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/screen_user_profile.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screen_user_profile.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17ScreenUserProfileE_t {};
} // unnamed namespace

template <> constexpr inline auto ScreenUserProfile::qt_create_metaobjectdata<qt_meta_tag_ZN17ScreenUserProfileE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ScreenUserProfile",
        "signalCloseUserProfile",
        "",
        "slotFillDataToForm",
        "slotClearDataOnForm",
        "on_cancelPushButton_clicked",
        "on_savePushButton_clicked",
        "on_nameLineEdit_editingFinished",
        "on_emailLineEdit_editingFinished",
        "on_phoneLineEdit_editingFinished",
        "on_passwordLineEdit_editingFinished",
        "on_confirnPasswordLineEdit_editingFinished",
        "on_changePasswordPushButton_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalCloseUserProfile'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotFillDataToForm'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotClearDataOnForm'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_cancelPushButton_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_savePushButton_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_nameLineEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_emailLineEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_phoneLineEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_passwordLineEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_confirnPasswordLineEdit_editingFinished'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_changePasswordPushButton_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ScreenUserProfile, qt_meta_tag_ZN17ScreenUserProfileE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ScreenUserProfile::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ScreenUserProfileE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ScreenUserProfileE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17ScreenUserProfileE_t>.metaTypes,
    nullptr
} };

void ScreenUserProfile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScreenUserProfile *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalCloseUserProfile(); break;
        case 1: _t->slotFillDataToForm(); break;
        case 2: _t->slotClearDataOnForm(); break;
        case 3: _t->on_cancelPushButton_clicked(); break;
        case 4: _t->on_savePushButton_clicked(); break;
        case 5: _t->on_nameLineEdit_editingFinished(); break;
        case 6: _t->on_emailLineEdit_editingFinished(); break;
        case 7: _t->on_phoneLineEdit_editingFinished(); break;
        case 8: _t->on_passwordLineEdit_editingFinished(); break;
        case 9: _t->on_confirnPasswordLineEdit_editingFinished(); break;
        case 10: _t->on_changePasswordPushButton_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ScreenUserProfile::*)()>(_a, &ScreenUserProfile::signalCloseUserProfile, 0))
            return;
    }
}

const QMetaObject *ScreenUserProfile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenUserProfile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17ScreenUserProfileE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ScreenUserProfile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void ScreenUserProfile::signalCloseUserProfile()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
