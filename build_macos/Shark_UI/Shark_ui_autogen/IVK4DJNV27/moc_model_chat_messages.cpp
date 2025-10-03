/****************************************************************************
** Meta object code from reading C++ file 'model_chat_messages.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/models/model_chat_messages.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'model_chat_messages.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12MessageModelE_t {};
} // unnamed namespace

template <> constexpr inline auto MessageModel::qt_create_metaobjectdata<qt_meta_tag_ZN12MessageModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MessageModel",
        "MessageRoles",
        "MessageTextRole",
        "SenderLoginRole",
        "SenderNameRole",
        "TimeStampRole",
        "MessageIdRole"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'MessageRoles'
        QtMocHelpers::EnumData<enum MessageRoles>(1, 1, QMC::EnumFlags{}).add({
            {    2, MessageRoles::MessageTextRole },
            {    3, MessageRoles::SenderLoginRole },
            {    4, MessageRoles::SenderNameRole },
            {    5, MessageRoles::TimeStampRole },
            {    6, MessageRoles::MessageIdRole },
        }),
    };
    return QtMocHelpers::metaObjectData<MessageModel, qt_meta_tag_ZN12MessageModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MessageModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12MessageModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12MessageModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12MessageModelE_t>.metaTypes,
    nullptr
} };

void MessageModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MessageModel *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *MessageModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MessageModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12MessageModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int MessageModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
