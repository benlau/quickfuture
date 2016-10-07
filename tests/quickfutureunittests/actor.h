#ifndef FILEACTOR_H
#define FILEACTOR_H

#include <QObject>
#include <QFuture>

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
};

#endif // FILEACTOR_H
