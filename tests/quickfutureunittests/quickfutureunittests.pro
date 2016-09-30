#-------------------------------------------------
#
# Project created by QtCreator 2016-02-25T18:56:34
#
#-------------------------------------------------

QT       += testlib qml concurrent

TARGET = quickfutureunittests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
ROOT_DIR = $$PWD/../..

SOURCES +=     main.cpp     \
    fileactor.cpp \
    quickfutureunittests.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(vendor/vendor.pri)
include($$ROOT_DIR/quickfuture.pri)

DISTFILES +=     qpm.json     qmltests/tst_QmlTests.qml \
    qmltests/test_typeChecking.qml

HEADERS +=     \
    fileactor.h \
    quickfutureunittests.h

