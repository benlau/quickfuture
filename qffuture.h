#ifndef QFFUTURE_H
#define QFFUTURE_H

#include <QObject>
#include <QFuture>
#include <QQmlEngine>
#include "qfvariantwrapper.h"

class QFFuture : public QObject
{
    Q_OBJECT
public:
    explicit QFFuture(QObject *parent = 0);

    template <typename T>
    static void registerType() {
        registerType(qRegisterMetaType<QFuture<T> >(), new QFVariantWrapper<T>() );
    }

    QJSEngine *engine() const;

    void setEngine(QQmlEngine *engine);

signals:

public slots:
    bool isFinished(const QVariant& future);

    bool isRunning(const QVariant& future);

    bool isCanceled(const QVariant& future);

    void onFinished(const QVariant& future, QJSValue func);

    void onCanceled(const QVariant& future, QJSValue func);

    QVariant result(const QVariant& future);

    QJSValue promise(QJSValue future);

    void sync(const QVariant& future, const QString& propertyInFuture, QObject* target, const QString& propertyInTarget = QString());

private:
    static void registerType(int typeId, QFVariantWrapperBase* wrapper);

    QPointer<QQmlEngine> m_engine;
    QJSValue promiseCreator;
};

#endif // QFFUTURE_H
