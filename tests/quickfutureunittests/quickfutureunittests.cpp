#include <QQmlApplicationEngine>
#include <QTest>
#include <Automator>
#include <QQmlContext>
#include "fileactor.h"
#include "quickfutureunittests.h"
#include "qfvariantwrapper.h"

Q_DECLARE_METATYPE(QFuture<QString>)

QuickFutureUnitTests::QuickFutureUnitTests(QObject *parent) : QObject(parent)
{

}

void QuickFutureUnitTests::typeChecking()
{
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("FileActor", new FileActor());
    engine.load(QString(SRCDIR) + "qmltests/test_typeChecking.qml");

}

void QuickFutureUnitTests::test_QFVariantWrapper()
{
    QFuture<QString> future;
    QVariant v = QVariant::fromValue<QFuture<QString> >(future);

    QVERIFY(future.isFinished());

    QFVariantWrapper<QString> wrapper;
    QVERIFY(wrapper.isFinished(v));


}

