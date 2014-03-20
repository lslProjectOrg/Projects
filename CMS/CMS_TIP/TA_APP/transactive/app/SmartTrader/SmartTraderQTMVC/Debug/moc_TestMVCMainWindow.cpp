/****************************************************************************
** Meta object code from reading C++ file 'TestMVCMainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../TestMVCMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TestMVCMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TestMVCMainWindow[] = {

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
      19,   18,   18,   18, 0x0a,
      35,   18,   18,   18, 0x08,
      61,   54,   49,   18, 0x08,
      87,   18,   49,   18, 0x28,
     102,   18,   18,   18, 0x08,
     114,   54,   49,   18, 0x08,
     140,   18,   49,   18, 0x28,
     155,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TestMVCMainWindow[] = {
    "TestMVCMainWindow\0\0updateActions()\0"
    "insertChild()\0bool\0parent\0"
    "insertColumn(QModelIndex)\0insertColumn()\0"
    "insertRow()\0removeColumn(QModelIndex)\0"
    "removeColumn()\0removeRow()\0"
};

void TestMVCMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TestMVCMainWindow *_t = static_cast<TestMVCMainWindow *>(_o);
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

const QMetaObjectExtraData TestMVCMainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TestMVCMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_TestMVCMainWindow,
      qt_meta_data_TestMVCMainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TestMVCMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TestMVCMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TestMVCMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TestMVCMainWindow))
        return static_cast<void*>(const_cast< TestMVCMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int TestMVCMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
