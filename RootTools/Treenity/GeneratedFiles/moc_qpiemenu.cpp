/****************************************************************************
** Meta object code from reading C++ file 'qpiemenu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Include/qpiemenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qpiemenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QClickablePixmapGraphicsItem_t {
    QByteArrayData data[3];
    char stringdata[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_QClickablePixmapGraphicsItem_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_QClickablePixmapGraphicsItem_t qt_meta_stringdata_QClickablePixmapGraphicsItem = {
    {
QT_MOC_LITERAL(0, 0, 28),
QT_MOC_LITERAL(1, 29, 7),
QT_MOC_LITERAL(2, 37, 0)
    },
    "QClickablePixmapGraphicsItem\0clicked\0"
    "\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QClickablePixmapGraphicsItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void QClickablePixmapGraphicsItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QClickablePixmapGraphicsItem *_t = static_cast<QClickablePixmapGraphicsItem *>(_o);
        switch (_id) {
        case 0: _t->clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QClickablePixmapGraphicsItem::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QClickablePixmapGraphicsItem::clicked)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QClickablePixmapGraphicsItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QClickablePixmapGraphicsItem.data,
      qt_meta_data_QClickablePixmapGraphicsItem,  qt_static_metacall, 0, 0}
};


const QMetaObject *QClickablePixmapGraphicsItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QClickablePixmapGraphicsItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QClickablePixmapGraphicsItem.stringdata))
        return static_cast<void*>(const_cast< QClickablePixmapGraphicsItem*>(this));
    if (!strcmp(_clname, "QGraphicsPixmapItem"))
        return static_cast< QGraphicsPixmapItem*>(const_cast< QClickablePixmapGraphicsItem*>(this));
    return QObject::qt_metacast(_clname);
}

int QClickablePixmapGraphicsItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QClickablePixmapGraphicsItem::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
struct qt_meta_stringdata_QButtonGraphicsItem_t {
    QByteArrayData data[5];
    char stringdata[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_QButtonGraphicsItem_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_QButtonGraphicsItem_t qt_meta_stringdata_QButtonGraphicsItem = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 7),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 4),
QT_MOC_LITERAL(4, 34, 3)
    },
    "QButtonGraphicsItem\0clicked\0\0hide\0pos\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QButtonGraphicsItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       1,   26, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

 // properties: name, type, flags
       4, QMetaType::QPointF, 0x00095103,

       0        // eod
};

void QButtonGraphicsItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QButtonGraphicsItem *_t = static_cast<QButtonGraphicsItem *>(_o);
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->hide(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QButtonGraphicsItem::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QButtonGraphicsItem::clicked)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QButtonGraphicsItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QButtonGraphicsItem.data,
      qt_meta_data_QButtonGraphicsItem,  qt_static_metacall, 0, 0}
};


const QMetaObject *QButtonGraphicsItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QButtonGraphicsItem::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QButtonGraphicsItem.stringdata))
        return static_cast<void*>(const_cast< QButtonGraphicsItem*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< QButtonGraphicsItem*>(this));
    return QObject::qt_metacast(_clname);
}

int QButtonGraphicsItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QPointF*>(_v) = pos(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPos(*reinterpret_cast< QPointF*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QButtonGraphicsItem::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
struct qt_meta_stringdata_QPieMenu_t {
    QByteArrayData data[14];
    char stringdata[151];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_QPieMenu_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_QPieMenu_t qt_meta_stringdata_QPieMenu = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 6),
QT_MOC_LITERAL(2, 16, 0),
QT_MOC_LITERAL(3, 17, 8),
QT_MOC_LITERAL(4, 26, 9),
QT_MOC_LITERAL(5, 36, 8),
QT_MOC_LITERAL(6, 45, 15),
QT_MOC_LITERAL(7, 61, 11),
QT_MOC_LITERAL(8, 73, 8),
QT_MOC_LITERAL(9, 82, 12),
QT_MOC_LITERAL(10, 95, 14),
QT_MOC_LITERAL(11, 110, 11),
QT_MOC_LITERAL(12, 122, 9),
QT_MOC_LITERAL(13, 132, 17)
    },
    "QPieMenu\0hidden\0\0showMenu\0closeMenu\0"
    "hideMenu\0hideAllPieMenus\0setNextMenu\0"
    "QWidget*\0showNextMenu\0setEasingCurve\0"
    "showMenuNow\0bgClicked\0checkActiveWindow\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QPieMenu[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       3,    0,   70,    2, 0x0a,
       4,    0,   71,    2, 0x0a,
       5,    0,   72,    2, 0x0a,
       6,    0,   73,    2, 0x0a,
       7,    1,   74,    2, 0x0a,
       9,    0,   77,    2, 0x0a,
      10,    1,   78,    2, 0x0a,
      11,    0,   81,    2, 0x08,
      12,    0,   82,    2, 0x08,
      13,    0,   83,    2, 0x08,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QEasingCurve,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QPieMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QPieMenu *_t = static_cast<QPieMenu *>(_o);
        switch (_id) {
        case 0: _t->hidden(); break;
        case 1: _t->showMenu(); break;
        case 2: _t->closeMenu(); break;
        case 3: _t->hideMenu(); break;
        case 4: _t->hideAllPieMenus(); break;
        case 5: _t->setNextMenu((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 6: _t->showNextMenu(); break;
        case 7: _t->setEasingCurve((*reinterpret_cast< const QEasingCurve(*)>(_a[1]))); break;
        case 8: _t->showMenuNow(); break;
        case 9: _t->bgClicked(); break;
        case 10: _t->checkActiveWindow(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QPieMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QPieMenu::hidden)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QPieMenu::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QPieMenu.data,
      qt_meta_data_QPieMenu,  qt_static_metacall, 0, 0}
};


const QMetaObject *QPieMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QPieMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPieMenu.stringdata))
        return static_cast<void*>(const_cast< QPieMenu*>(this));
    return QWidget::qt_metacast(_clname);
}

int QPieMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void QPieMenu::hidden()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
