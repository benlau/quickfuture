#ifndef QFVARIANTWRAPPER_H
#define QFVARIANTWRAPPER_H

#include <QVariant>
#include <QFuture>
#include <QJSValue>
#include <QFutureWatcher>

class QFVariantWrapperBase {
public:
    virtual bool isFinished(const QVariant& v) = 0;
    virtual void onFinished(const QVariant& v, QJSValue& func) = 0;
};

template <typename T>
class QFVariantWrapper : public QFVariantWrapperBase {
public:

    virtual bool isFinished(const QVariant& v) {
        QFuture<T> future = v.value<QFuture<T> >();
        return future.isFinished();
    }

    virtual void onFinished(const QVariant& v, QJSValue& func) {
        if (!func.isCallable()) {
            qWarning() << "Future.onFinished: Callback is not callable";
            return;
        }

        QFuture<T> future = v.value<QFuture<T> >();
        QFutureWatcher<T> *watcher = 0;

        auto listener = [=]() {
            //@FIXME - call with arguments
            //@FIXME - If exception is raised. Report the exception
            QJSValue callback = func;
            callback.call();
            if (watcher != 0) {
                delete watcher;
            }
        };

        if (future.isFinished()) {
            //@FIXME - call listener by queued connection
            listener();
        } else {
            watcher = new QFutureWatcher<T>();
            watcher->setFuture(future);
            QObject::connect(watcher, &QFutureWatcherBase::finished, listener);
        }
    }

};

#endif // QFVARIANTWRAPPER_H
