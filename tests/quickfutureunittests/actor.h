#ifndef FILEACTOR_H
#define FILEACTOR_H

#include <QObject>
#include <QFuture>
#include <QSize>

class Actor : public QObject
{
    Q_OBJECT
public:
    explicit Actor(QObject *parent = 0);

signals:

public slots:

    QFuture<QString> read(const QString& fileName);

    QFuture<void> dummy();

    QFuture<void> alreadyFinished();

    QFuture<void> canceled();

    QFuture<bool> delayReturnBool(bool value);

    QFuture<QSize> delayReturnQSize(QSize value);
};

#endif // FILEACTOR_H
