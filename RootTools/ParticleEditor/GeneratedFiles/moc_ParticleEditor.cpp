/****************************************************************************
** Meta object code from reading C++ file 'ParticleEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/ParticleEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ParticleEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ParticleEditor_t {
    QByteArrayData data[12];
    char stringdata[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ParticleEditor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ParticleEditor_t qt_meta_stringdata_ParticleEditor = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 7),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 8),
QT_MOC_LITERAL(4, 33, 8),
QT_MOC_LITERAL(5, 42, 10),
QT_MOC_LITERAL(6, 53, 8),
QT_MOC_LITERAL(7, 62, 13),
QT_MOC_LITERAL(8, 76, 13),
QT_MOC_LITERAL(9, 90, 17),
QT_MOC_LITERAL(10, 108, 13),
QT_MOC_LITERAL(11, 122, 13)
    },
    "ParticleEditor\0MenuNew\0\0MenuOpen\0"
    "MenuSave\0ManuSaveAs\0MenuExit\0MenuViewStats\0"
    "MenuHelpAbout\0MenuCreateEmitter\0"
    "DeleteEmitter\0RenameEmitter\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ParticleEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08,
       3,    0,   65,    2, 0x08,
       4,    0,   66,    2, 0x08,
       5,    0,   67,    2, 0x08,
       6,    0,   68,    2, 0x08,
       7,    0,   69,    2, 0x08,
       8,    0,   70,    2, 0x08,
       9,    0,   71,    2, 0x08,
      10,    0,   72,    2, 0x08,
      11,    0,   73,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ParticleEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ParticleEditor *_t = static_cast<ParticleEditor *>(_o);
        switch (_id) {
        case 0: _t->MenuNew(); break;
        case 1: _t->MenuOpen(); break;
        case 2: _t->MenuSave(); break;
        case 3: _t->ManuSaveAs(); break;
        case 4: _t->MenuExit(); break;
        case 5: _t->MenuViewStats(); break;
        case 6: _t->MenuHelpAbout(); break;
        case 7: _t->MenuCreateEmitter(); break;
        case 8: _t->DeleteEmitter(); break;
        case 9: _t->RenameEmitter(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ParticleEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ParticleEditor.data,
      qt_meta_data_ParticleEditor,  qt_static_metacall, 0, 0}
};


const QMetaObject *ParticleEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ParticleEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ParticleEditor.stringdata))
        return static_cast<void*>(const_cast< ParticleEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ParticleEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
