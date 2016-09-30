#include <QQmlApplicationEngine>
#include <QTest>
#include <Automator>
#include <QQmlContext>
#include "fileactor.h"
#include "quickfutureunittests.h"

QuickFutureUnitTests::QuickFutureUnitTests(QObject *parent) : QObject(parent)
{

}

void QuickFutureUnitTests::typeChecking()
{
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("FileActor", new FileActor());
    engine.load(QString(SRCDIR) + "qmltests/test_typeChecking.qml");

}

