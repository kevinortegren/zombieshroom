/****************************************************************************
** Meta object code from reading C++ file 'TransformView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/Components/TransformView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TransformView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TransformView_t {
    QByteArrayData data[12];
    char stringdata[177];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_TransformView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_TransformView_t qt_meta_stringdata_TransformView = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 16),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 7),
QT_MOC_LITERAL(4, 40, 16),
QT_MOC_LITERAL(5, 57, 16),
QT_MOC_LITERAL(6, 74, 19),
QT_MOC_LITERAL(7, 94, 19),
QT_MOC_LITERAL(8, 114, 19),
QT_MOC_LITERAL(9, 134, 13),
QT_MOC_LITERAL(10, 148, 13),
QT_MOC_LITERAL(11, 162, 13)
    },
    "TransformView\0PositionXChanged\0\0p_value\0"
    "PositionYChanged\0PositionZChanged\0"
    "OrientationXChanged\0OrientationYChanged\0"
    "OrientationZChanged\0ScaleXChanged\0"
    "ScaleYChanged\0ScaleZChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TransformView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x08,
       4,    1,   62,    2, 0x08,
       5,    1,   65,    2, 0x08,
       6,    1,   68,    2, 0x08,
       7,    1,   71,    2, 0x08,
       8,    1,   74,    2, 0x08,
       9,    1,   77,    2, 0x08,
      10,    1,   80,    2, 0x08,
      11,    1,   83,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,

       0        // eod
};

void TransformView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TransformView *_t = static_cast<TransformView *>(_o);
        switch (_id) {
        case 0: _t->PositionXChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->PositionYChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->PositionZChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->OrientationXChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->OrientationYChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->OrientationZChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->ScaleXChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->ScaleYChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->ScaleZChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject TransformView::staticMetaObject = {
    { &AbstractComponentView::staticMetaObject, qt_meta_stringdata_TransformView.data,
      qt_meta_data_TransformView,  qt_static_metacall, 0, 0}
};


const QMetaObject *TransformView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TransformView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TransformView.stringdata))
        return static_cast<void*>(const_cast< TransformView*>(this));
    return AbstractComponentView::qt_metacast(_clname);
}

int TransformView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractComponentView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
