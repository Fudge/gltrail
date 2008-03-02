/****************************************************************************
** Meta object code from reading C++ file 'host.h'
**
** Created: Sun Mar 2 16:34:32 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "host.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'host.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Host[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x0a,
      23,    5,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Host[] = {
    "Host\0\0readFromStdout()\0readFromStderr()\0"
};

const QMetaObject Host::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Host,
      qt_meta_data_Host, 0 }
};

const QMetaObject *Host::metaObject() const
{
    return &staticMetaObject;
}

void *Host::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Host))
	return static_cast<void*>(const_cast< Host*>(this));
    return QObject::qt_metacast(_clname);
}

int Host::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: readFromStdout(); break;
        case 1: readFromStderr(); break;
        }
        _id -= 2;
    }
    return _id;
}
