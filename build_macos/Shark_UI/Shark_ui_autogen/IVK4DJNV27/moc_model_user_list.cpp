/****************************************************************************
** Meta object code from reading C++ file 'model_user_list.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/models/model_user_list.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'model_user_list.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13UserListModelE_t {};
} // unnamed namespace

template <> constexpr inline auto UserListModel::qt_create_metaobjectdata<qt_meta_tag_ZN13UserListModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "UserListModel",
        "UserListRoles",
        "LoginRole",
        "NameRole",
        "EmailRole",
        "PhoneRole",
        "DisableReasonRole",
        "IsActiveRole",
        "DisableAtRole",
        "BunUntilRole"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'UserListRoles'
        QtMocHelpers::EnumData<enum UserListRoles>(1, 1, QMC::EnumFlags{}).add({
            {    2, UserListRoles::LoginRole },
            {    3, UserListRoles::NameRole },
            {    4, UserListRoles::EmailRole },
            {    5, UserListRoles::PhoneRole },
            {    6, UserListRoles::DisableReasonRole },
            {    7, UserListRoles::IsActiveRole },
            {    8, UserListRoles::DisableAtRole },
            {    9, UserListRoles::BunUntilRole },
        }),
    };
    return QtMocHelpers::metaObjectData<UserListModel, qt_meta_tag_ZN13UserListModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject UserListModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13UserListModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13UserListModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13UserListModelE_t>.metaTypes,
    nullptr
} };

void UserListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<UserListModel *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *UserListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13UserListModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int UserListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
