#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T18:56:34
#
#-------------------------------------------------

QT       += testlib qml concurrent
CONFIG += c++11

TARGET = quickfutureunittests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
ROOT_DIR = $$PWD/../..

SOURCES +=     main.cpp     \
    quickfutureunittests.cpp \
    actor.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(vendor/vendor.pri)
include($$ROOT_DIR/quickfuture.pri)

DISTFILES +=     qpm.json     \
    ../../README.md \
    qmltests/PromiseIsNotInstalled.qml \
    qmltests/tst_Callback.qml \
    qmltests/tst_Promise.qml \
    qmltests/tst_Sync.qml \
    qmltests/CustomTestCase.qml

HEADERS +=     \
    quickfutureunittests.h \
    actor.h

