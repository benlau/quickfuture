#ifndef QFFUTURE_H
#define QFFUTURE_H

#include <QObject>
#include <QFuture>
#include <QJSEngine>
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

    void setEngine(QJSEngine *engine);

signals:

public slots:
    bool isFinished(const QVariant& future);

    void onFinished(const QVariant& future, QJSValue func);

private:
    static void registerType(int typeId, QFVariantWrapperBase* wrapper);

    QPointer<QJSEngine> m_engine;
};

#endif // QFFUTURE_H
