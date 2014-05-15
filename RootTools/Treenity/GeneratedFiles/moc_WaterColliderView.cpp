/****************************************************************************
** Meta object code from reading C++ file 'WaterColliderView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/Components/WaterColliderView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WaterColliderView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_WaterColliderView_t {
    QByteArrayData data[6];
    char stringdata[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_WaterColliderView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_WaterColliderView_t qt_meta_stringdata_WaterColliderView = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 13),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 7),
QT_MOC_LITERAL(4, 41, 22),
QT_MOC_LITERAL(5, 64, 19)
    },
    "WaterColliderView\0RadiusChanged\0\0"
    "p_value\0DisturbIntervalChanged\0"
    "DisturbPowerChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WaterColliderView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x08,
       4,    1,   32,    2, 0x08,
       5,    1,   35,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,

       0        // eod
};

void WaterColliderView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WaterColliderView *_t = static_cast<WaterColliderView *>(_o);
        switch (_id) {
        case 0: _t->RadiusChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->DisturbIntervalChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->DisturbPowerChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject WaterColliderView::staticMetaObject = {
    { &AbstractComponentView::staticMetaObject, qt_meta_stringdata_WaterColliderView.data,
      qt_meta_data_WaterColliderView,  qt_static_metacall, 0, 0}
};


const QMetaObject *WaterColliderView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WaterColliderView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WaterColliderView.stringdata))
        return static_cast<void*>(const_cast< WaterColliderView*>(this));
    return AbstractComponentView::qt_metacast(_clname);
}

int WaterColliderView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractComponentView::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
