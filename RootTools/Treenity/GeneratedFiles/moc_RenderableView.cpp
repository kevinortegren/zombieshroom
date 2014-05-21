/****************************************************************************
** Meta object code from reading C++ file 'RenderableView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/Components/RenderableView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RenderableView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RenderableView_t {
    QByteArrayData data[12];
    char stringdata[178];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_RenderableView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_RenderableView_t qt_meta_stringdata_RenderableView = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 11),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 19),
QT_MOC_LITERAL(4, 48, 16),
QT_MOC_LITERAL(5, 65, 13),
QT_MOC_LITERAL(6, 79, 5),
QT_MOC_LITERAL(7, 85, 21),
QT_MOC_LITERAL(8, 107, 6),
QT_MOC_LITERAL(9, 114, 22),
QT_MOC_LITERAL(10, 137, 18),
QT_MOC_LITERAL(11, 156, 20)
    },
    "RenderableView\0NewMaterial\0\0"
    "MaterialNameChanged\0ModelNameChanged\0"
    "ShaderChanged\0index\0DiffuseTextureDropped\0"
    "p_path\0SpecularTextureDropped\0"
    "GlowTextureDropped\0NormalTextureDropped\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderableView[] = {

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
       1,    0,   54,    2, 0x08,
       3,    0,   55,    2, 0x08,
       4,    0,   56,    2, 0x08,
       5,    1,   57,    2, 0x08,
       7,    1,   60,    2, 0x08,
       9,    1,   63,    2, 0x08,
      10,    1,   66,    2, 0x08,
      11,    1,   69,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,

       0        // eod
};

void RenderableView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RenderableView *_t = static_cast<RenderableView *>(_o);
        switch (_id) {
        case 0: _t->NewMaterial(); break;
        case 1: _t->MaterialNameChanged(); break;
        case 2: _t->ModelNameChanged(); break;
        case 3: _t->ShaderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->DiffuseTextureDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->SpecularTextureDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->GlowTextureDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->NormalTextureDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject RenderableView::staticMetaObject = {
    { &AbstractComponentView::staticMetaObject, qt_meta_stringdata_RenderableView.data,
      qt_meta_data_RenderableView,  qt_static_metacall, 0, 0}
};


const QMetaObject *RenderableView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderableView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RenderableView.stringdata))
        return static_cast<void*>(const_cast< RenderableView*>(this));
    return AbstractComponentView::qt_metacast(_clname);
}

int RenderableView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractComponentView::qt_metacall(_c, _id, _a);
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
