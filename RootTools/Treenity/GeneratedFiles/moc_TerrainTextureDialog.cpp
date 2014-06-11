/****************************************************************************
** Meta object code from reading C++ file 'TerrainTextureDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/TerrainTextureDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TerrainTextureDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TerrainTextureDialog_t {
    QByteArrayData data[10];
    char stringdata[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_TerrainTextureDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_TerrainTextureDialog_t qt_meta_stringdata_TerrainTextureDialog = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 4),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 11),
QT_MOC_LITERAL(4, 39, 7),
QT_MOC_LITERAL(5, 47, 5),
QT_MOC_LITERAL(6, 53, 10),
QT_MOC_LITERAL(7, 64, 10),
QT_MOC_LITERAL(8, 75, 12),
QT_MOC_LITERAL(9, 88, 11)
    },
    "TerrainTextureDialog\0Show\0\0DisplayData\0"
    "SetSize\0p_val\0SetOpacity\0RedClicked\0"
    "GreenClicked\0BlueClicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TerrainTextureDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a,
       3,    0,   50,    2, 0x08,
       4,    1,   51,    2, 0x08,
       6,    1,   54,    2, 0x08,
       7,    0,   57,    2, 0x08,
       8,    0,   58,    2, 0x08,
       9,    0,   59,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TerrainTextureDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TerrainTextureDialog *_t = static_cast<TerrainTextureDialog *>(_o);
        switch (_id) {
        case 0: _t->Show(); break;
        case 1: _t->DisplayData(); break;
        case 2: _t->SetSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->SetOpacity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->RedClicked(); break;
        case 5: _t->GreenClicked(); break;
        case 6: _t->BlueClicked(); break;
        default: ;
        }
    }
}

const QMetaObject TerrainTextureDialog::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TerrainTextureDialog.data,
      qt_meta_data_TerrainTextureDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *TerrainTextureDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TerrainTextureDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TerrainTextureDialog.stringdata))
        return static_cast<void*>(const_cast< TerrainTextureDialog*>(this));
    return QWidget::qt_metacast(_clname);
}

int TerrainTextureDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
