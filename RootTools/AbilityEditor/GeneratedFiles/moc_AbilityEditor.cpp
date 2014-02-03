/****************************************************************************
** Meta object code from reading C++ file 'AbilityEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/AbilityEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AbilityEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AbilityEditor_t {
    QByteArrayData data[13];
    char stringdata[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_AbilityEditor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_AbilityEditor_t qt_meta_stringdata_AbilityEditor = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 21),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 14),
QT_MOC_LITERAL(4, 52, 6),
QT_MOC_LITERAL(5, 59, 4),
QT_MOC_LITERAL(6, 64, 4),
QT_MOC_LITERAL(7, 69, 12),
QT_MOC_LITERAL(8, 82, 12),
QT_MOC_LITERAL(9, 95, 10),
QT_MOC_LITERAL(10, 106, 17),
QT_MOC_LITERAL(11, 124, 11),
QT_MOC_LITERAL(12, 136, 10)
    },
    "AbilityEditor\0UpdatePropertyBrowser\0"
    "\0GenerateScript\0SaveAs\0Save\0Load\0"
    "FileViewDrag\0p_modelIndex\0BrowseName\0"
    "ChangeAbilityName\0SetCooldown\0p_cooldown\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AbilityEditor[] = {

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
       1,    0,   59,    2, 0x08,
       3,    0,   60,    2, 0x08,
       4,    0,   61,    2, 0x08,
       5,    0,   62,    2, 0x08,
       6,    0,   63,    2, 0x08,
       7,    1,   64,    2, 0x08,
       9,    0,   67,    2, 0x08,
      10,    0,   68,    2, 0x08,
      11,    1,   69,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   12,

       0        // eod
};

void AbilityEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AbilityEditor *_t = static_cast<AbilityEditor *>(_o);
        switch (_id) {
        case 0: _t->UpdatePropertyBrowser(); break;
        case 1: _t->GenerateScript(); break;
        case 2: _t->SaveAs(); break;
        case 3: _t->Save(); break;
        case 4: _t->Load(); break;
        case 5: _t->FileViewDrag((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: _t->BrowseName(); break;
        case 7: _t->ChangeAbilityName(); break;
        case 8: _t->SetCooldown((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject AbilityEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_AbilityEditor.data,
      qt_meta_data_AbilityEditor,  qt_static_metacall, 0, 0}
};


const QMetaObject *AbilityEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AbilityEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AbilityEditor.stringdata))
        return static_cast<void*>(const_cast< AbilityEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int AbilityEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
