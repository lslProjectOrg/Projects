/****************************************************************************
** Meta object code from reading C++ file 'SmartTraderQTMVCMainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../SmartTraderQTMVCMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SmartTraderQTMVCMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SmartTraderQTMVCMainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x0a,
      44,   27,   27,   27, 0x08,
      70,   63,   58,   27, 0x08,
      96,   27,   58,   27, 0x28,
     111,   27,   27,   27, 0x08,
     123,   63,   58,   27, 0x08,
     149,   27,   58,   27, 0x28,
     164,   27,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SmartTraderQTMVCMainWindow[] = {
    "SmartTraderQTMVCMainWindow\0\0updateActions()\0"
    "insertChild()\0bool\0parent\0"
    "insertColumn(QModelIndex)\0insertColumn()\0"
    "insertRow()\0removeColumn(QModelIndex)\0"
    "removeColumn()\0removeRow()\0"
};

void SmartTraderQTMVCMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SmartTraderQTMVCMainWindow *_t = static_cast<SmartTraderQTMVCMainWindow *>(_o);
        switch (_id) {
        case 0: _t->updateActions(); break;
        case 1: _t->insertChild(); break;
        case 2: { bool _r = _t->insertColumn((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->insertColumn();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->insertRow(); break;
        case 5: { bool _r = _t->removeColumn((*reinterpret_cast< const QModelIndex(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->removeColumn();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->removeRow(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SmartTraderQTMVCMainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SmartTraderQTMVCMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_SmartTraderQTMVCMainWindow,
      qt_meta_data_SmartTraderQTMVCMainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SmartTraderQTMVCMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SmartTraderQTMVCMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SmartTraderQTMVCMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SmartTraderQTMVCMainWindow))
        return static_cast<void*>(const_cast< SmartTraderQTMVCMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int SmartTraderQTMVCMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
