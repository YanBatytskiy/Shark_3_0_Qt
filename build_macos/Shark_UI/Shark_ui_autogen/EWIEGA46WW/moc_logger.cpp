/****************************************************************************
** Meta object code from reading C++ file 'logger.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Shark_UI/logger.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logger.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN6LoggerE_t {};
} // unnamed namespace

template <> constexpr inline auto Logger::qt_create_metaobjectdata<qt_meta_tag_ZN6LoggerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Logger",
        "signalWriteLine",
        "",
        "logLine",
        "signalReadLine",
        "indexLine",
        "signalStopLogger",
        "slotStopLogger",
        "slotWriteLine",
        "slotReadLastLine",
        "std::multimap<qint64,QString>",
        "slotReadSeveralLines",
        "linesToRead",
        "slotClearLogFile",
        "slotgetLineCount"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signalWriteLine'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'signalReadLine'
        QtMocHelpers::SignalData<void(qint64)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 5 },
        }}),
        // Signal 'signalStopLogger'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotStopLogger'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slotWriteLine'
        QtMocHelpers::SlotData<bool(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 3 },
        }}),
        // Slot 'slotReadLastLine'
        QtMocHelpers::SlotData<std::multimap<qint64,QString>()>(9, 2, QMC::AccessPublic, 0x80000000 | 10),
        // Slot 'slotReadSeveralLines'
        QtMocHelpers::SlotData<std::multimap<qint64,QString>(qint64)>(11, 2, QMC::AccessPublic, 0x80000000 | 10, {{
            { QMetaType::LongLong, 12 },
        }}),
        // Slot 'slotClearLogFile'
        QtMocHelpers::SlotData<bool()>(13, 2, QMC::AccessPublic, QMetaType::Bool),
        // Slot 'slotgetLineCount'
        QtMocHelpers::SlotData<qint64()>(14, 2, QMC::AccessPrivate, QMetaType::LongLong),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Logger, qt_meta_tag_ZN6LoggerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Logger::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6LoggerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6LoggerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6LoggerE_t>.metaTypes,
    nullptr
} };

void Logger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Logger *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signalWriteLine((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->signalReadLine((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 2: _t->signalStopLogger(); break;
        case 3: _t->slotStopLogger(); break;
        case 4: { bool _r = _t->slotWriteLine((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { std::multimap<qint64,QString> _r = _t->slotReadLastLine();
            if (_a[0]) *reinterpret_cast< std::multimap<qint64,QString>*>(_a[0]) = std::move(_r); }  break;
        case 6: { std::multimap<qint64,QString> _r = _t->slotReadSeveralLines((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast< std::multimap<qint64,QString>*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->slotClearLogFile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { qint64 _r = _t->slotgetLineCount();
            if (_a[0]) *reinterpret_cast< qint64*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Logger::*)(const QString & )>(_a, &Logger::signalWriteLine, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Logger::*)(qint64 )>(_a, &Logger::signalReadLine, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Logger::*)()>(_a, &Logger::signalStopLogger, 2))
            return;
    }
}

const QMetaObject *Logger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Logger::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6LoggerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Logger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void Logger::signalWriteLine(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void Logger::signalReadLine(qint64 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Logger::signalStopLogger()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
