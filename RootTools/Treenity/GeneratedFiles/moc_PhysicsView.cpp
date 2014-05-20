/****************************************************************************
** Meta object code from reading C++ file 'PhysicsView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/Components/PhysicsView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PhysicsView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PhysicsView_t {
    QByteArrayData data[18];
    char stringdata[297];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_PhysicsView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_PhysicsView_t qt_meta_stringdata_PhysicsView = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 11),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 7),
QT_MOC_LITERAL(4, 33, 25),
QT_MOC_LITERAL(5, 59, 26),
QT_MOC_LITERAL(6, 86, 15),
QT_MOC_LITERAL(7, 102, 15),
QT_MOC_LITERAL(8, 118, 15),
QT_MOC_LITERAL(9, 134, 11),
QT_MOC_LITERAL(10, 146, 12),
QT_MOC_LITERAL(11, 159, 19),
QT_MOC_LITERAL(12, 179, 17),
QT_MOC_LITERAL(13, 197, 17),
QT_MOC_LITERAL(14, 215, 21),
QT_MOC_LITERAL(15, 237, 21),
QT_MOC_LITERAL(16, 259, 18),
QT_MOC_LITERAL(17, 278, 17)
    },
    "PhysicsView\0TypeChanged\0\0p_value\0"
    "CollisionWithWorldChanged\0"
    "CollisionWithStaticChanged\0GravityXChanged\0"
    "GravityYChanged\0GravityZChanged\0"
    "MassChanged\0ShapeChanged\0SphereRadiusChanged\0"
    "ConeRadiusChanged\0ConeHeightChanged\0"
    "CylinderRadiusChanged\0CylinderHeightChanged\0"
    "PhysicsMeshChanged\0PhysicsMeshBrowse\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PhysicsView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x08,
       4,    1,   92,    2, 0x08,
       5,    1,   95,    2, 0x08,
       6,    1,   98,    2, 0x08,
       7,    1,  101,    2, 0x08,
       8,    1,  104,    2, 0x08,
       9,    1,  107,    2, 0x08,
      10,    1,  110,    2, 0x08,
      11,    1,  113,    2, 0x08,
      12,    1,  116,    2, 0x08,
      13,    1,  119,    2, 0x08,
      14,    1,  122,    2, 0x08,
      15,    1,  125,    2, 0x08,
      16,    0,  128,    2, 0x08,
      17,    0,  129,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PhysicsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PhysicsView *_t = static_cast<PhysicsView *>(_o);
        switch (_id) {
        case 0: _t->TypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->CollisionWithWorldChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->CollisionWithStaticChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->GravityXChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->GravityYChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->GravityZChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->MassChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->ShapeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->SphereRadiusChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->ConeRadiusChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->ConeHeightChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->CylinderRadiusChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->CylinderHeightChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->PhysicsMeshChanged(); break;
        case 14: _t->PhysicsMeshBrowse(); break;
        default: ;
        }
    }
}

const QMetaObject PhysicsView::staticMetaObject = {
    { &AbstractComponentView::staticMetaObject, qt_meta_stringdata_PhysicsView.data,
      qt_meta_data_PhysicsView,  qt_static_metacall, 0, 0}
};


const QMetaObject *PhysicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PhysicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PhysicsView.stringdata))
        return static_cast<void*>(const_cast< PhysicsView*>(this));
    return AbstractComponentView::qt_metacast(_clname);
}

int PhysicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractComponentView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
