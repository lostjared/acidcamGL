/****************************************************************************
** Meta object code from reading C++ file 'sock_server.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "sock_server.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sock_server.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ServerThread_t {
    const uint offsetsAndSize[10];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ServerThread_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ServerThread_t qt_meta_stringdata_ServerThread = {
    {
QT_MOC_LITERAL(0, 12), // "ServerThread"
QT_MOC_LITERAL(13, 3), // "Log"
QT_MOC_LITERAL(17, 0), // ""
QT_MOC_LITERAL(18, 4), // "text"
QT_MOC_LITERAL(23, 7) // "process"

    },
    "ServerThread\0Log\0\0text\0process"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ServerThread[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   29,    2, 0x0a,    3 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ServerThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ServerThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Log((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->process(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ServerThread::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ServerThread::Log)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ServerThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_ServerThread.offsetsAndSize,
    qt_meta_data_ServerThread,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ServerThread_t
, QtPrivate::TypeAndForceComplete<ServerThread, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *ServerThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ServerThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ServerThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int ServerThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ServerThread::Log(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
