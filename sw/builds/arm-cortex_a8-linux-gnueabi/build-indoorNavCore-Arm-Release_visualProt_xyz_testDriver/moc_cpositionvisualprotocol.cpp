/****************************************************************************
** Meta object code from reading C++ file 'cpositionvisualprotocol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../indoorNavCore/HAL/cpositionvisualprotocol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cpositionvisualprotocol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cPositionVisualProtocol_t {
    QByteArrayData data[6];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cPositionVisualProtocol_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cPositionVisualProtocol_t qt_meta_stringdata_cPositionVisualProtocol = {
    {
QT_MOC_LITERAL(0, 0, 23), // "cPositionVisualProtocol"
QT_MOC_LITERAL(1, 24, 23), // "on_server_newConnection"
QT_MOC_LITERAL(2, 48, 0), // ""
QT_MOC_LITERAL(3, 49, 19), // "on_socket_readyRead"
QT_MOC_LITERAL(4, 69, 19), // "on_socket_connected"
QT_MOC_LITERAL(5, 89, 22) // "on_socket_disconnected"

    },
    "cPositionVisualProtocol\0on_server_newConnection\0"
    "\0on_socket_readyRead\0on_socket_connected\0"
    "on_socket_disconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cPositionVisualProtocol[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void cPositionVisualProtocol::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cPositionVisualProtocol *_t = static_cast<cPositionVisualProtocol *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_server_newConnection(); break;
        case 1: _t->on_socket_readyRead(); break;
        case 2: _t->on_socket_connected(); break;
        case 3: _t->on_socket_disconnected(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject cPositionVisualProtocol::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_cPositionVisualProtocol.data,
      qt_meta_data_cPositionVisualProtocol,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *cPositionVisualProtocol::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cPositionVisualProtocol::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_cPositionVisualProtocol.stringdata0))
        return static_cast<void*>(const_cast< cPositionVisualProtocol*>(this));
    if (!strcmp(_clname, "cProtocolDriver"))
        return static_cast< cProtocolDriver*>(const_cast< cPositionVisualProtocol*>(this));
    return QObject::qt_metacast(_clname);
}

int cPositionVisualProtocol::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
