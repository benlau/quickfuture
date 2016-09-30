#include <QtQml>

//@TODO move to header file
Q_DECLARE_METATYPE(QFuture<QString>)

static void init() {
    qRegisterMetaType<QFuture<QString> >();
}

Q_COREAPP_STARTUP_FUNCTION(init)
