/****************************************************************************
** Meta object code from reading C++ file 'SynThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../cmlFpTestApp/ChameleonQt/src/SynThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SynThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SynThread_t {
    QByteArrayData data[12];
    char stringdata0[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SynThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SynThread_t qt_meta_stringdata_SynThread = {
    {
QT_MOC_LITERAL(0, 0, 9), // "SynThread"
QT_MOC_LITERAL(1, 10, 12), // "sendTestStep"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 12), // "EngineNumber"
QT_MOC_LITERAL(4, 37, 6), // "TestID"
QT_MOC_LITERAL(5, 44, 11), // "strTestStep"
QT_MOC_LITERAL(6, 56, 20), // "const SynTestResult*"
QT_MOC_LITERAL(7, 77, 11), // "pTestResult"
QT_MOC_LITERAL(8, 89, 2), // "rc"
QT_MOC_LITERAL(9, 92, 14), // "sendTestResult"
QT_MOC_LITERAL(10, 107, 4), // "Type"
QT_MOC_LITERAL(11, 112, 9) // "finalStep"

    },
    "SynThread\0sendTestStep\0\0EngineNumber\0"
    "TestID\0strTestStep\0const SynTestResult*\0"
    "pTestResult\0rc\0sendTestResult\0Type\0"
    "finalStep"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SynThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   29,    2, 0x06 /* Public */,
       9,    3,   40,    2, 0x06 /* Public */,
       9,    3,   47,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::QString, 0x80000000 | 6, QMetaType::UInt,    3,    4,    5,    7,    8,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 6, QMetaType::Int,    3,    7,   10,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 6, QMetaType::Bool,    3,    7,   11,

       0        // eod
};

void SynThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SynThread *_t = static_cast<SynThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendTestStep((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const SynTestResult*(*)>(_a[4])),(*reinterpret_cast< uint(*)>(_a[5]))); break;
        case 1: _t->sendTestResult((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const SynTestResult*(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 2: _t->sendTestResult((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const SynTestResult*(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SynThread::*_t)(unsigned int , unsigned int , const QString , const SynTestResult * , unsigned int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SynThread::sendTestStep)) {
                *result = 0;
            }
        }
        {
            typedef void (SynThread::*_t)(unsigned int , const SynTestResult * , const int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SynThread::sendTestResult)) {
                *result = 1;
            }
        }
        {
            typedef void (SynThread::*_t)(unsigned int , const SynTestResult * , const bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SynThread::sendTestResult)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject SynThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SynThread.data,
      qt_meta_data_SynThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SynThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SynThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SynThread.stringdata0))
        return static_cast<void*>(const_cast< SynThread*>(this));
    return QThread::qt_metacast(_clname);
}

int SynThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SynThread::sendTestStep(unsigned int _t1, unsigned int _t2, const QString _t3, const SynTestResult * _t4, unsigned int _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SynThread::sendTestResult(unsigned int _t1, const SynTestResult * _t2, const int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SynThread::sendTestResult(unsigned int _t1, const SynTestResult * _t2, const bool _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
