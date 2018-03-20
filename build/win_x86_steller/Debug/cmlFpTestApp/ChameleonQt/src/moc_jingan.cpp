/****************************************************************************
** Meta object code from reading C++ file 'jingan.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../cmlFpTestApp/ChameleonQt/src/jingan.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jingan.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Jingan_t {
    QByteArrayData data[19];
    char stringdata0[214];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Jingan_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Jingan_t qt_meta_stringdata_Jingan = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Jingan"
QT_MOC_LITERAL(1, 7, 4), // "Exit"
QT_MOC_LITERAL(2, 12, 0), // ""
QT_MOC_LITERAL(3, 13, 10), // "Initialize"
QT_MOC_LITERAL(4, 24, 19), // "CreateLocalSettings"
QT_MOC_LITERAL(5, 44, 17), // "DisplayInfomation"
QT_MOC_LITERAL(6, 62, 3), // "Run"
QT_MOC_LITERAL(7, 66, 15), // "ReceiveTestStep"
QT_MOC_LITERAL(8, 82, 12), // "EngineNumber"
QT_MOC_LITERAL(9, 95, 6), // "TestID"
QT_MOC_LITERAL(10, 102, 11), // "strTestStep"
QT_MOC_LITERAL(11, 114, 20), // "const SynTestResult*"
QT_MOC_LITERAL(12, 135, 11), // "pTestResult"
QT_MOC_LITERAL(13, 147, 2), // "rc"
QT_MOC_LITERAL(14, 150, 17), // "ReceiveTestResult"
QT_MOC_LITERAL(15, 168, 9), // "finalStep"
QT_MOC_LITERAL(16, 178, 14), // "ReceivingImage"
QT_MOC_LITERAL(17, 193, 9), // "pTestData"
QT_MOC_LITERAL(18, 203, 10) // "strDutType"

    },
    "Jingan\0Exit\0\0Initialize\0CreateLocalSettings\0"
    "DisplayInfomation\0Run\0ReceiveTestStep\0"
    "EngineNumber\0TestID\0strTestStep\0"
    "const SynTestResult*\0pTestResult\0rc\0"
    "ReceiveTestResult\0finalStep\0ReceivingImage\0"
    "pTestData\0strDutType"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Jingan[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    5,   59,    2, 0x0a /* Public */,
      14,    3,   70,    2, 0x0a /* Public */,
      16,    3,   77,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::QString, 0x80000000 | 11, QMetaType::UInt,    8,    9,   10,   12,   13,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 11, QMetaType::Bool,    8,   12,   15,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 11, QMetaType::QString,    8,   17,   18,

       0        // eod
};

void Jingan::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Jingan *_t = static_cast<Jingan *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Exit(); break;
        case 1: _t->Initialize(); break;
        case 2: _t->CreateLocalSettings(); break;
        case 3: _t->DisplayInfomation(); break;
        case 4: _t->Run(); break;
        case 5: _t->ReceiveTestStep((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const SynTestResult*(*)>(_a[4])),(*reinterpret_cast< uint(*)>(_a[5]))); break;
        case 6: _t->ReceiveTestResult((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const SynTestResult*(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        case 7: _t->ReceivingImage((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< const SynTestResult*(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject Jingan::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Jingan.data,
      qt_meta_data_Jingan,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Jingan::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Jingan::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Jingan.stringdata0))
        return static_cast<void*>(const_cast< Jingan*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Jingan::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
