TARGET = quickfuture
TEMPLATE = lib

isEmpty(SHARED) {
    SHARED = "false"
}

isEmpty(INSTALL_ROOT) {
    INSTALL_ROOT = $$[QT_INSTALL_LIBS]
}

target.path = $${INSTALL_ROOT}

include(../quickfuture.pri)

equals(SHARED, "false") {
    CONFIG += staticlib
}
