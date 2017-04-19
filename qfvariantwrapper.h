#ifndef QFVARIANTWRAPPER_H
#define QFVARIANTWRAPPER_H

#include <QVariant>
#include <QFuture>
#include <QJSValue>
#include <QFutureWatcher>
#include <QPointer>
#include <QJSEngine>
#include <QCoreApplication>

namespace QuickFuture {

    template <typename T>
    inline QJSValueList valueList(const QPointer<QQmlEngine>& engine, const QFuture<T>& future) {
        QJSValue value = engine->toScriptValue<T>(future.result());
        return QJSValueList() << value;
    }

    template <>
    inline QJSValueList valueList<void>(const QPointer<QQmlEngine>& engine, const QFuture<void>& future) {
        Q_UNUSED(engine);
        Q_UNUSED(future);
        return QJSValueList();
    }

    template <typename F>
    inline void nextTick(F func) {
        QObject tmp;
        QObject::connect(&tmp, &QObject::destroyed, QCoreApplication::instance(), func, Qt::QueuedConnection);
    }

    template <typename T>
    inline QVariant toVariant(const QFuture<T> &future) {
        if (!future.isResultReadyAt(0)) {
            qWarning() << "Future.result(): The result is not ready!";
            return QVariant();
        }

        return QVariant::fromValue<T>(future.result());
    }

    template <>
    inline QVariant toVariant<void>(const QFuture<void> &future) {
        Q_UNUSED(future);
        return QVariant();
    }

class VariantWrapperBase {
public:
    virtual inline ~VariantWrapperBase() {
    }

    virtual bool isPaused(const QVariant& v) = 0;
    virtual bool isFinished(const QVariant& v) = 0;
    virtual bool isRunning(const QVariant& v) = 0;
    virtual bool isCanceled(const QVariant& v) = 0;

    virtual QVariant result(const QVariant& v) = 0;

    virtual void onFinished(QPointer<QQmlEngine> engine, const QVariant& v, const QJSValue& func) = 0;

    virtual void onCanceled(QPointer<QQmlEngine> engine, const QVariant& v, const QJSValue& func) = 0;

    virtual void sync(const QVariant &v, const QString &propertyInFuture, QObject *target, const QString &propertyInTarget) = 0;


    bool property(const QVariant& v, const QString& name) {
        bool res = false;
        if (name == "isFinished") {
            res = isFinished(v);
        } else if (name == "isRunning") {
            res = isRunning(v);
        } else if (name == "isPaused") {
            res = isPaused(v);
        } else {
            qWarning().noquote() << QString("Future: Unknown property: %1").arg(name);
        }
        return res;
    }    
};

#define QF_WRAPPER_DECL_READ(type, method) \
    virtual type method(const QVariant& v) { \
        QFuture<T> future = v.value<QFuture<T> >();\
        return future.method(); \
    }

#define QF_WRAPPER_CHECK_CALLABLE(method, func) \
    if (!func.isCallable()) { \
        qWarning() << "Future." #method ": Callback is not callable"; \
        return; \
    }

#define QF_WRAPPER_CONNECT(method, checker) \
        virtual void method(QPointer<QQmlEngine> engine, const QVariant& v, const QJSValue& func) { \
        if (!func.isCallable()) { \
            qWarning() << "Future." #method ": Callback is not callable"; \
            return; \
        } \
        QFuture<T> future = v.value<QFuture<T> >(); \
        QFutureWatcher<T> *watcher = 0; \
        auto listener = [=]() { \
            if (!engine.isNull()) { \
                QJSValue callback = func; \
                QJSValue ret = callback.call(QuickFuture::valueList<T>(engine, future)); \
                if (ret.isError()) { \
                    QString message = QString("%1:%2: %3: %4") \
                                      .arg(ret.property("fileName").toString()) \
                                      .arg(ret.property("lineNumber").toString()) \
                                      .arg(ret.property("name").toString()) \
                                      .arg(ret.property("message").toString()); \
                    qWarning() << message; \
                } \
            } \
            if (watcher != 0) { \
                delete watcher; \
            } \
        };\
        if (future.checker()) { \
            QuickFuture::nextTick(listener); \
        } else { \
            watcher = new QFutureWatcher<T>(); \
            QObject::connect(watcher, &QFutureWatcherBase::finished, listener); \
            watcher->setFuture(future); \
        } \
    }

template <typename T>
class VariantWrapper : public VariantWrapperBase {
public:

    QF_WRAPPER_DECL_READ(bool, isFinished)

    QF_WRAPPER_DECL_READ(bool, isRunning)

    QF_WRAPPER_DECL_READ(bool, isPaused)

    QF_WRAPPER_DECL_READ(bool, isCanceled)

    QF_WRAPPER_CONNECT(onFinished, isFinished)

    QF_WRAPPER_CONNECT(onCanceled, isCanceled)

    QVariant result(const QVariant &future) {
        QFuture<T> f = future.value<QFuture<T>>();
        return QuickFuture::toVariant(f);
    }

    void sync(const QVariant &future, const QString &propertyInFuture, QObject *target, const QString &propertyInTarget) {
        QPointer<QObject> object = target;
        QString pt = propertyInTarget;
        if (pt.isEmpty()) {
            pt = propertyInFuture;
        }

        auto setProperty = [=]() {
            if (object.isNull()) {
                return;
            }
            bool value = property(future, propertyInFuture);
            object->setProperty( pt.toUtf8().constData(), value);
        };

        setProperty();
        QFuture<T> f = future.value<QFuture<T> >();

        if (f.isFinished()) {
            // No need to listen on an already finished future
            return;
        }

        QFutureWatcher<T> *watcher = new QFutureWatcher<T>();

        QObject::connect(watcher, &QFutureWatcherBase::canceled, setProperty);
        QObject::connect(watcher, &QFutureWatcherBase::paused, setProperty);
        QObject::connect(watcher, &QFutureWatcherBase::resumed, setProperty);
        QObject::connect(watcher, &QFutureWatcherBase::started, setProperty);

        QObject::connect(watcher,  &QFutureWatcherBase::finished, [=]() {
            setProperty();
            watcher->deleteLater();
        });

        watcher->setFuture(f);
    }
};

} // End of namespace

#endif // QFVARIANTWRAPPER_H
