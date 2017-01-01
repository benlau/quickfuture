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

    void onFinished(const QVariant& future, QJSValue func);

    QJSValue promise(QJSValue future);

private:
    static void registerType(int typeId, QFVariantWrapperBase* wrapper);

    QPointer<QQmlEngine> m_engine;
    QJSValue promiseCreator;
};

#endif // QFFUTURE_H
