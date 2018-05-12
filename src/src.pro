TARGET = quickfuture
TEMPLATE = lib

isEmpty(SHARED) {
    SHARED = "false"
}

isEmpty(INSTALL_ROOT) {
    INSTALL_ROOT = $$[QT_INSTALL_LIBS]
}

target.path = $${INSTALL_ROOT}/lib
INSTALLS += target

include(../quickfuture.pri)

headers.files += \
    $$PWD/qfvariantwrapper.h \
    $$PWD/qffuture.h \
    $$PWD/QuickFuture \
    $$PWD/quickfuture.h

headers.path = $${INSTALL_ROOT}/include
INSTALLS += headers

equals(SHARED, "false") {
    CONFIG += staticlib
}
