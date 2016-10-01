#ifndef QFVARIANTWRAPPER_H
#define QFVARIANTWRAPPER_H

#include <QVariant>
#include <QFuture>

class QFVariantWrapperBase {
public:

};

template <typename T>
class QFVariantWrapper : public QFVariantWrapperBase {
public:

    virtual bool isFinished(const QVariant& v) {
        QFuture<T> future = v.value<QFuture<T> >();
        return future.isFinished();
    }

};

#endif // QFVARIANTWRAPPER_H
