/****************************************************************************
** Meta object code from reading C++ file 'WaterTool.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/WaterTool.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WaterTool.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WaterTool_t {
    QByteArrayData data[18];
    char stringdata[211];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_WaterTool_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_WaterTool_t qt_meta_stringdata_WaterTool = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 4),
QT_MOC_LITERAL(2, 15, 0),
QT_MOC_LITERAL(3, 16, 13),
QT_MOC_LITERAL(4, 30, 7),
QT_MOC_LITERAL(5, 38, 13),
QT_MOC_LITERAL(6, 52, 7),
QT_MOC_LITERAL(7, 60, 19),
QT_MOC_LITERAL(8, 80, 8),
QT_MOC_LITERAL(9, 89, 19),
QT_MOC_LITERAL(10, 109, 15),
QT_MOC_LITERAL(11, 125, 13),
QT_MOC_LITERAL(12, 139, 13),
QT_MOC_LITERAL(13, 153, 7),
QT_MOC_LITERAL(14, 161, 10),
QT_MOC_LITERAL(15, 172, 9),
QT_MOC_LITERAL(16, 182, 11),
QT_MOC_LITERAL(17, 194, 15)
    },
    "WaterTool\0Show\0\0SetWaterSpeed\0p_speed\0"
    "SetWaterDepth\0p_depth\0SetWaterRefractions\0"
    "p_active\0SetWaterReflections\0"
    "SetWaterNormals\0SetWaterPause\0"
    "SetWaterColor\0p_color\0ResetWater\0"
    "InitWater\0DisplayData\0OpenColorDialog\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WaterTool[] = {

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
       3,    1,   75,    2, 0x08,
       5,    1,   78,    2, 0x08,
       7,    1,   81,    2, 0x08,
       9,    1,   84,    2, 0x08,
      10,    1,   87,    2, 0x08,
      11,    1,   90,    2, 0x08,
      12,    1,   93,    2, 0x08,
      14,    0,   96,    2, 0x08,
      15,    0,   97,    2, 0x08,
      16,    0,   98,    2, 0x08,
      17,    0,   99,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::QColor,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void WaterTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WaterTool *_t = static_cast<WaterTool *>(_o);
        switch (_id) {
        case 0: _t->Show(); break;
        case 1: _t->SetWaterSpeed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->SetWaterDepth((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->SetWaterRefractions((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->SetWaterReflections((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->SetWaterNormals((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->SetWaterPause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->SetWaterColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 8: _t->ResetWater(); break;
        case 9: _t->InitWater(); break;
        case 10: _t->DisplayData(); break;
        case 11: _t->OpenColorDialog(); break;
        default: ;
        }
    }
}

const QMetaObject WaterTool::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_WaterTool.data,
      qt_meta_data_WaterTool,  qt_static_metacall, 0, 0}
};


const QMetaObject *WaterTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WaterTool::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WaterTool.stringdata))
        return static_cast<void*>(const_cast< WaterTool*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int WaterTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
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
