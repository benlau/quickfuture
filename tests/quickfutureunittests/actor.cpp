#include <QtConcurrent>
#include <Automator>
#include "actor.h"

Actor::Actor(QObject *parent) : QObject(parent)
{

}

QFuture<QString> Actor::read(const QString &fileName)
{
    return QtConcurrent::run([fileName]() -> QString {

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            return "";
        }

        return file.readAll();
    });
}

QFuture<void> Actor::dummy()
{
    return QtConcurrent::run([]() -> void {
        Automator::wait(100);
        return;
    });
}

QFuture<void> Actor::alreadyFinished()
{
    return QFuture<void>();
}
