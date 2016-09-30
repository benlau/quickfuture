#ifndef FILEACTOR_H
#define FILEACTOR_H

#include <QObject>
#include <QFuture>

class FileActor : public QObject
{
    Q_OBJECT
public:
    explicit FileActor(QObject *parent = 0);

signals:

public slots:

    QFuture<QString> read(const QString& fileName);

};

#endif // FILEACTOR_H
