/****************************************************************************
** Meta object code from reading C++ file 'LocalSettings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../cmlFpTestApp/ChameleonQt/src/LocalSettings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LocalSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LocalSettings_t {
    QByteArrayData data[16];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LocalSettings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LocalSettings_t qt_meta_stringdata_LocalSettings = {
    {
QT_MOC_LITERAL(0, 0, 13), // "LocalSettings"
QT_MOC_LITERAL(1, 14, 14), // "dragEnterEvent"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 16), // "QDragEnterEvent*"
QT_MOC_LITERAL(4, 47, 5), // "event"
QT_MOC_LITERAL(5, 53, 9), // "dropEvent"
QT_MOC_LITERAL(6, 63, 11), // "QDropEvent*"
QT_MOC_LITERAL(7, 75, 16), // "SelectConfigFile"
QT_MOC_LITERAL(8, 92, 17), // "SelectLogFilePath"
QT_MOC_LITERAL(9, 110, 22), // "ModifyTestEngineCounts"
QT_MOC_LITERAL(10, 133, 1), // "i"
QT_MOC_LITERAL(11, 135, 21), // "CreateDeviceManagment"
QT_MOC_LITERAL(12, 157, 16), // "ButtonBoxClicked"
QT_MOC_LITERAL(13, 174, 16), // "QAbstractButton*"
QT_MOC_LITERAL(14, 191, 6), // "button"
QT_MOC_LITERAL(15, 198, 7) // "Confirm"

    },
    "LocalSettings\0dragEnterEvent\0\0"
    "QDragEnterEvent*\0event\0dropEvent\0"
    "QDropEvent*\0SelectConfigFile\0"
    "SelectLogFilePath\0ModifyTestEngineCounts\0"
    "i\0CreateDeviceManagment\0ButtonBoxClicked\0"
    "QAbstractButton*\0button\0Confirm"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LocalSettings[] = {

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
       1,    1,   54,    2, 0x0a /* Public */,
       5,    1,   57,    2, 0x0a /* Public */,
       7,    0,   60,    2, 0x0a /* Public */,
       8,    0,   61,    2, 0x0a /* Public */,
       9,    1,   62,    2, 0x0a /* Public */,
      11,    0,   65,    2, 0x0a /* Public */,
      12,    1,   66,    2, 0x0a /* Public */,
      15,    0,   69,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,

       0        // eod
};

void LocalSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LocalSettings *_t = static_cast<LocalSettings *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dragEnterEvent((*reinterpret_cast< QDragEnterEvent*(*)>(_a[1]))); break;
        case 1: _t->dropEvent((*reinterpret_cast< QDropEvent*(*)>(_a[1]))); break;
        case 2: _t->SelectConfigFile(); break;
        case 3: _t->SelectLogFilePath(); break;
        case 4: _t->ModifyTestEngineCounts((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->CreateDeviceManagment(); break;
        case 6: _t->ButtonBoxClicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 7: _t->Confirm(); break;
        default: ;
        }
    }
}

const QMetaObject LocalSettings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LocalSettings.data,
      qt_meta_data_LocalSettings,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LocalSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LocalSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LocalSettings.stringdata0))
        return static_cast<void*>(const_cast< LocalSettings*>(this));
    return QDialog::qt_metacast(_clname);
}

int LocalSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
