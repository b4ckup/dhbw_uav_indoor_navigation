/****************************************************************************
** Meta object code from reading C++ file 'cfiltercontrol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../indoorNavCore/FPL/cfiltercontrol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cfiltercontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_cFilterControl_t {
    QByteArrayData data[5];
    char stringdata0[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_cFilterControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_cFilterControl_t qt_meta_stringdata_cFilterControl = {
    {
QT_MOC_LITERAL(0, 0, 14), // "cFilterControl"
QT_MOC_LITERAL(1, 15, 23), // "on_beaconParameter_rcvd"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 23), // "cBeaconSignalParameter*"
QT_MOC_LITERAL(4, 64, 3) // "par"

    },
    "cFilterControl\0on_beaconParameter_rcvd\0"
    "\0cBeaconSignalParameter*\0par"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_cFilterControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void cFilterControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        cFilterControl *_t = static_cast<cFilterControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_beaconParameter_rcvd((*reinterpret_cast< cBeaconSignalParameter*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject cFilterControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_cFilterControl.data,
      qt_meta_data_cFilterControl,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *cFilterControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cFilterControl::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_cFilterControl.stringdata0))
        return static_cast<void*>(const_cast< cFilterControl*>(this));
    return QObject::qt_metacast(_clname);
}

int cFilterControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
