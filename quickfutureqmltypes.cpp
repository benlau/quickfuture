#include <QtQml>
#include "qffuture.h"

//@TODO move to header file
Q_DECLARE_METATYPE(QFuture<QString>)
Q_DECLARE_METATYPE(QFuture<int>)

static void init() {
    QFFuture::registerType<QString>();
    QFFuture::registerType<int>();
}

Q_COREAPP_STARTUP_FUNCTION(init)
