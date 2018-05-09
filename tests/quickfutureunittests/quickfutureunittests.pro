QT       += testlib qml concurrent
CONFIG += c++11

TARGET = quickfutureunittests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
ROOT_DIR = $$PWD/../..
INCLUDEPATH += $$PWD/../..

SOURCES +=     main.cpp     \
    quickfutureunittests.cpp \
    actor.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(vendor/vendor.pri)

DEFINES += QUICK_TEST_SOURCE_DIR=\\\"$$PWD\\\"

DISTFILES +=     qpm.json     \
    ../../README.md \
    qmltests/PromiseIsNotInstalled.qml \
    qmltests/tst_Callback.qml \
    qmltests/tst_Promise.qml \
    qmltests/tst_Sync.qml \
    qmltests/CustomTestCase.qml \
    ../../qpm.json \
    ../../.travis.yml \
    ../../appveyor.yml

HEADERS +=     \
    quickfutureunittests.h \
    actor.h

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

win32 {
    message($${OUT_PWD})

    CONFIG(release) {
        LIBS += -L${OUT_PWD}../../../src/Release
    } else {
        LIBS += -L${OUT_PWD}../../../src/Debug
    }

} else {
    LIBS += -L${OUT_PWD}../../src
}

LIBS += -lquickfuture
