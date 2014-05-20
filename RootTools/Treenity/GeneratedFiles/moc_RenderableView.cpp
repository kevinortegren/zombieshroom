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
    QByteArrayData data[9];
    char stringdata[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_RenderableView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_RenderableView_t qt_meta_stringdata_RenderableView = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 19),
QT_MOC_LITERAL(2, 35, 0),
QT_MOC_LITERAL(3, 36, 16),
QT_MOC_LITERAL(4, 53, 21),
QT_MOC_LITERAL(5, 75, 6),
QT_MOC_LITERAL(6, 82, 22),
QT_MOC_LITERAL(7, 105, 18),
QT_MOC_LITERAL(8, 124, 20)
    },
    "RenderableView\0MaterialNameChanged\0\0"
    "ModelNameChanged\0DiffuseTextureDropped\0"
    "p_path\0SpecularTextureDropped\0"
    "GlowTextureDropped\0NormalTextureDropped\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderableView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08,
       3,    0,   45,    2, 0x08,
       4,    1,   46,    2, 0x08,
       6,    1,   49,    2, 0x08,
       7,    1,   52,    2, 0x08,
       8,    1,   55,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void RenderableView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RenderableView *_t = static_cast<RenderableView *>(_o);
        switch (_id) {
        case 0: _t->MaterialNameChanged(); break;
        case 1: _t->ModelNameChanged(); break;
        case 2: _t->DiffuseTextureDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->SpecularTextureDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->GlowTextureDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->NormalTextureDropped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
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
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
