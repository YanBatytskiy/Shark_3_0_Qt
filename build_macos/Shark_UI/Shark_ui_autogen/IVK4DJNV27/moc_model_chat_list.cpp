/****************************************************************************
** Meta object code from reading C++ file 'model_chat_list.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/models/model_chat_list.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'model_chat_list.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13ChatListModelE_t {};
} // unnamed namespace

template <> constexpr inline auto ChatListModel::qt_create_metaobjectdata<qt_meta_tag_ZN13ChatListModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ChatListModel",
        "ChatListRoles",
        "ParticipantsChatListRole",
        "InfoTextRole",
        "UnreadCountRole",
        "IsMutedRole",
        "LastTimeRole",
        "ChatIdRole"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'ChatListRoles'
        QtMocHelpers::EnumData<enum ChatListRoles>(1, 1, QMC::EnumFlags{}).add({
            {    2, ChatListRoles::ParticipantsChatListRole },
            {    3, ChatListRoles::InfoTextRole },
            {    4, ChatListRoles::UnreadCountRole },
            {    5, ChatListRoles::IsMutedRole },
            {    6, ChatListRoles::LastTimeRole },
            {    7, ChatListRoles::ChatIdRole },
        }),
    };
    return QtMocHelpers::metaObjectData<ChatListModel, qt_meta_tag_ZN13ChatListModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ChatListModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ChatListModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ChatListModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13ChatListModelE_t>.metaTypes,
    nullptr
} };

void ChatListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChatListModel *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *ChatListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ChatListModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int ChatListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
