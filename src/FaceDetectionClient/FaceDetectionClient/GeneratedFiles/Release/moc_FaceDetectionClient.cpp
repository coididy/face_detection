/****************************************************************************
** Meta object code from reading C++ file 'FaceDetectionClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../FaceDetectionClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FaceDetectionClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FaceDetectionClient_t {
    QByteArrayData data[19];
    char stringdata0[298];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FaceDetectionClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FaceDetectionClient_t qt_meta_stringdata_FaceDetectionClient = {
    {
QT_MOC_LITERAL(0, 0, 19), // "FaceDetectionClient"
QT_MOC_LITERAL(1, 20, 12), // "sendSrcImage"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "image"
QT_MOC_LITERAL(4, 40, 13), // "sendTestImage"
QT_MOC_LITERAL(5, 54, 8), // "sendStop"
QT_MOC_LITERAL(6, 63, 21), // "systemTrayIconClicked"
QT_MOC_LITERAL(7, 85, 33), // "QSystemTrayIcon::ActivationRe..."
QT_MOC_LITERAL(8, 119, 6), // "reason"
QT_MOC_LITERAL(9, 126, 15), // "videoBtnClicked"
QT_MOC_LITERAL(10, 142, 14), // "videoLabelShow"
QT_MOC_LITERAL(11, 157, 16), // "cameraBtnClicked"
QT_MOC_LITERAL(12, 174, 20), // "photographBtnClicked"
QT_MOC_LITERAL(13, 195, 23), // "edgeDetectionBtnClicked"
QT_MOC_LITERAL(14, 219, 21), // "videoRecordBtnClicked"
QT_MOC_LITERAL(15, 241, 18), // "openFileBtnClicked"
QT_MOC_LITERAL(16, 260, 17), // "faceConBtnClicked"
QT_MOC_LITERAL(17, 278, 14), // "getThreadError"
QT_MOC_LITERAL(18, 293, 4) // "flag"

    },
    "FaceDetectionClient\0sendSrcImage\0\0"
    "image\0sendTestImage\0sendStop\0"
    "systemTrayIconClicked\0"
    "QSystemTrayIcon::ActivationReason\0"
    "reason\0videoBtnClicked\0videoLabelShow\0"
    "cameraBtnClicked\0photographBtnClicked\0"
    "edgeDetectionBtnClicked\0videoRecordBtnClicked\0"
    "openFileBtnClicked\0faceConBtnClicked\0"
    "getThreadError\0flag"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FaceDetectionClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       5,    0,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   86,    2, 0x08 /* Private */,
       9,    0,   89,    2, 0x08 /* Private */,
      10,    0,   90,    2, 0x08 /* Private */,
      11,    0,   91,    2, 0x08 /* Private */,
      12,    0,   92,    2, 0x08 /* Private */,
      13,    0,   93,    2, 0x08 /* Private */,
      14,    0,   94,    2, 0x08 /* Private */,
      15,    0,   95,    2, 0x08 /* Private */,
      16,    0,   96,    2, 0x08 /* Private */,
      17,    1,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,

       0        // eod
};

void FaceDetectionClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FaceDetectionClient *_t = static_cast<FaceDetectionClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendSrcImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->sendTestImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->sendStop(); break;
        case 3: _t->systemTrayIconClicked((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 4: _t->videoBtnClicked(); break;
        case 5: _t->videoLabelShow(); break;
        case 6: _t->cameraBtnClicked(); break;
        case 7: _t->photographBtnClicked(); break;
        case 8: _t->edgeDetectionBtnClicked(); break;
        case 9: _t->videoRecordBtnClicked(); break;
        case 10: _t->openFileBtnClicked(); break;
        case 11: _t->faceConBtnClicked(); break;
        case 12: _t->getThreadError((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FaceDetectionClient::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FaceDetectionClient::sendSrcImage)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FaceDetectionClient::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FaceDetectionClient::sendTestImage)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FaceDetectionClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FaceDetectionClient::sendStop)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FaceDetectionClient::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FaceDetectionClient.data,
      qt_meta_data_FaceDetectionClient,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FaceDetectionClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FaceDetectionClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FaceDetectionClient.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FaceDetectionClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void FaceDetectionClient::sendSrcImage(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FaceDetectionClient::sendTestImage(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FaceDetectionClient::sendStop()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
