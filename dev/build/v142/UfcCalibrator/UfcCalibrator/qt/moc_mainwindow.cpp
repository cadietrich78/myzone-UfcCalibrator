/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../source/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QResizeableTreeView_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QResizeableTreeView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QResizeableTreeView_t qt_meta_stringdata_QResizeableTreeView = {
    {
QT_MOC_LITERAL(0, 0, 19) // "QResizeableTreeView"

    },
    "QResizeableTreeView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QResizeableTreeView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void QResizeableTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QResizeableTreeView::staticMetaObject = { {
    &QTreeView::staticMetaObject,
    qt_meta_stringdata_QResizeableTreeView.data,
    qt_meta_data_QResizeableTreeView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QResizeableTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QResizeableTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QResizeableTreeView.stringdata0))
        return static_cast<void*>(this);
    return QTreeView::qt_metacast(_clname);
}

int QResizeableTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata0[336];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 7), // "Repaint"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 7), // "Refresh"
QT_MOC_LITERAL(4, 28, 15), // "RefreshContents"
QT_MOC_LITERAL(5, 44, 14), // "OpenVideoFrame"
QT_MOC_LITERAL(6, 59, 15), // "OpenWebcamFrame"
QT_MOC_LITERAL(7, 75, 24), // "SaveFightFlowCalibration"
QT_MOC_LITERAL(8, 100, 17), // "CloseGameCallback"
QT_MOC_LITERAL(9, 118, 12), // "EditSettings"
QT_MOC_LITERAL(10, 131, 16), // "ClearCalibration"
QT_MOC_LITERAL(11, 148, 21), // "PlaybackPreviousFrame"
QT_MOC_LITERAL(12, 170, 18), // "PlaybackPlayButton"
QT_MOC_LITERAL(13, 189, 23), // "PlaybackNextFrameButton"
QT_MOC_LITERAL(14, 213, 22), // "PlaybackPositionSlider"
QT_MOC_LITERAL(15, 236, 8), // "position"
QT_MOC_LITERAL(16, 245, 15), // "PlaybackRefresh"
QT_MOC_LITERAL(17, 261, 12), // "IdleCallback"
QT_MOC_LITERAL(18, 274, 20), // "BackFromIdleCallback"
QT_MOC_LITERAL(19, 295, 14), // "ShowCheatSheet"
QT_MOC_LITERAL(20, 310, 7), // "ShowLog"
QT_MOC_LITERAL(21, 318, 17) // "CurrentTabChanged"

    },
    "MainWindow\0Repaint\0\0Refresh\0RefreshContents\0"
    "OpenVideoFrame\0OpenWebcamFrame\0"
    "SaveFightFlowCalibration\0CloseGameCallback\0"
    "EditSettings\0ClearCalibration\0"
    "PlaybackPreviousFrame\0PlaybackPlayButton\0"
    "PlaybackNextFrameButton\0PlaybackPositionSlider\0"
    "position\0PlaybackRefresh\0IdleCallback\0"
    "BackFromIdleCallback\0ShowCheatSheet\0"
    "ShowLog\0CurrentTabChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  114,    2, 0x0a /* Public */,
       3,    0,  115,    2, 0x0a /* Public */,
       4,    0,  116,    2, 0x08 /* Private */,
       5,    0,  117,    2, 0x08 /* Private */,
       6,    0,  118,    2, 0x08 /* Private */,
       7,    0,  119,    2, 0x08 /* Private */,
       8,    0,  120,    2, 0x08 /* Private */,
       9,    0,  121,    2, 0x08 /* Private */,
      10,    0,  122,    2, 0x08 /* Private */,
      11,    0,  123,    2, 0x08 /* Private */,
      12,    0,  124,    2, 0x08 /* Private */,
      13,    0,  125,    2, 0x08 /* Private */,
      14,    1,  126,    2, 0x08 /* Private */,
      16,    0,  129,    2, 0x08 /* Private */,
      17,    0,  130,    2, 0x08 /* Private */,
      18,    0,  131,    2, 0x08 /* Private */,
      19,    0,  132,    2, 0x08 /* Private */,
      20,    0,  133,    2, 0x08 /* Private */,
      21,    0,  134,    2, 0x08 /* Private */,
      21,    1,  135,    2, 0x08 /* Private */,

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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Repaint(); break;
        case 1: _t->Refresh(); break;
        case 2: _t->RefreshContents(); break;
        case 3: _t->OpenVideoFrame(); break;
        case 4: _t->OpenWebcamFrame(); break;
        case 5: _t->SaveFightFlowCalibration(); break;
        case 6: _t->CloseGameCallback(); break;
        case 7: _t->EditSettings(); break;
        case 8: _t->ClearCalibration(); break;
        case 9: _t->PlaybackPreviousFrame(); break;
        case 10: _t->PlaybackPlayButton(); break;
        case 11: _t->PlaybackNextFrameButton(); break;
        case 12: _t->PlaybackPositionSlider((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->PlaybackRefresh(); break;
        case 14: _t->IdleCallback(); break;
        case 15: _t->BackFromIdleCallback(); break;
        case 16: _t->ShowCheatSheet(); break;
        case 17: _t->ShowLog(); break;
        case 18: _t->CurrentTabChanged(); break;
        case 19: _t->CurrentTabChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
