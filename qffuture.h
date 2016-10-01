#ifndef QFFUTURE_H
#define QFFUTURE_H

#include <QObject>
#include <QFuture>
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
signals:

public slots:
    bool isFinished(const QVariant& future);

private:
    static void registerType(int typeId, QFVariantWrapperBase* wrapper);

};

#endif // QFFUTURE_H
