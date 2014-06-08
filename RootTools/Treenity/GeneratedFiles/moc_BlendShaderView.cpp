/****************************************************************************
** Meta object code from reading C++ file 'BlendShaderView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/Shaders/BlendShaderView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BlendShaderView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BlendShaderView_t {
    QByteArrayData data[9];
    char stringdata[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_BlendShaderView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_BlendShaderView_t qt_meta_stringdata_BlendShaderView = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 14),
QT_MOC_LITERAL(2, 31, 0),
QT_MOC_LITERAL(3, 32, 25),
QT_MOC_LITERAL(4, 58, 9),
QT_MOC_LITERAL(5, 68, 40),
QT_MOC_LITERAL(6, 109, 8),
QT_MOC_LITERAL(7, 118, 17),
QT_MOC_LITERAL(8, 136, 5)
    },
    "BlendShaderView\0TextureChanged\0\0"
    "Render::TextureInterface*\0p_texture\0"
    "Render::TextureSemantic::TextureSemantic\0"
    "p_texSem\0TileFactorChanged\0p_val\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BlendShaderView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x08,
       7,    1,   29,    2, 0x08,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, QMetaType::Int,    8,

       0        // eod
};

void BlendShaderView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BlendShaderView *_t = static_cast<BlendShaderView *>(_o);
        switch (_id) {
        case 0: _t->TextureChanged((*reinterpret_cast< Render::TextureInterface*(*)>(_a[1])),(*reinterpret_cast< Render::TextureSemantic::TextureSemantic(*)>(_a[2]))); break;
        case 1: _t->TileFactorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject BlendShaderView::staticMetaObject = {
    { &AbstractShaderView::staticMetaObject, qt_meta_stringdata_BlendShaderView.data,
      qt_meta_data_BlendShaderView,  qt_static_metacall, 0, 0}
};


const QMetaObject *BlendShaderView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BlendShaderView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BlendShaderView.stringdata))
        return static_cast<void*>(const_cast< BlendShaderView*>(this));
    return AbstractShaderView::qt_metacast(_clname);
}

int BlendShaderView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AbstractShaderView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
