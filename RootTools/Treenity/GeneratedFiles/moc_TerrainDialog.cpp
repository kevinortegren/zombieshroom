/****************************************************************************
** Meta object code from reading C++ file 'TerrainDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/TerrainDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TerrainDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TerrainDialog_t {
    QByteArrayData data[15];
    char stringdata[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_TerrainDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_TerrainDialog_t qt_meta_stringdata_TerrainDialog = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 4),
QT_MOC_LITERAL(2, 19, 0),
QT_MOC_LITERAL(3, 20, 11),
QT_MOC_LITERAL(4, 32, 7),
QT_MOC_LITERAL(5, 40, 5),
QT_MOC_LITERAL(6, 46, 11),
QT_MOC_LITERAL(7, 58, 13),
QT_MOC_LITERAL(8, 72, 14),
QT_MOC_LITERAL(9, 87, 12),
QT_MOC_LITERAL(10, 100, 10),
QT_MOC_LITERAL(11, 111, 9),
QT_MOC_LITERAL(12, 121, 10),
QT_MOC_LITERAL(13, 132, 10),
QT_MOC_LITERAL(14, 143, 10)
    },
    "TerrainDialog\0Show\0\0DisplayData\0SetSize\0"
    "p_val\0SetStrength\0SetAutoSmooth\0"
    "SetCircleStyle\0SetGridStyle\0SetOptions\0"
    "SmoothAll\0PieButton1\0PieButton2\0"
    "PieButton3\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TerrainDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a,
       3,    0,   75,    2, 0x08,
       4,    1,   76,    2, 0x08,
       6,    1,   79,    2, 0x08,
       7,    1,   82,    2, 0x08,
       8,    0,   85,    2, 0x08,
       9,    0,   86,    2, 0x08,
      10,    1,   87,    2, 0x08,
      11,    0,   90,    2, 0x08,
      12,    0,   91,    2, 0x08,
      13,    0,   92,    2, 0x08,
      14,    0,   93,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TerrainDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TerrainDialog *_t = static_cast<TerrainDialog *>(_o);
        switch (_id) {
        case 0: _t->Show(); break;
        case 1: _t->DisplayData(); break;
        case 2: _t->SetSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->SetStrength((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->SetAutoSmooth((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->SetCircleStyle(); break;
        case 6: _t->SetGridStyle(); break;
        case 7: _t->SetOptions((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->SmoothAll(); break;
        case 9: _t->PieButton1(); break;
        case 10: _t->PieButton2(); break;
        case 11: _t->PieButton3(); break;
        default: ;
        }
    }
}

const QMetaObject TerrainDialog::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TerrainDialog.data,
      qt_meta_data_TerrainDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *TerrainDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TerrainDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TerrainDialog.stringdata))
        return static_cast<void*>(const_cast< TerrainDialog*>(this));
    return QWidget::qt_metacast(_clname);
}

int TerrainDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
