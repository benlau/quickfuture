#include <QQmlApplicationEngine>
#include <QTest>
#include <Automator>
#include <QQmlContext>
#include <QtShell>
#include <QuickFuture>
#include <QtConcurrent>
#include "actor.h"
#include "quickfutureunittests.h"

Q_DECLARE_METATYPE(QFuture<QString>)

template <typename F>
static bool waitUntil(F f, int timeout = -1) {
    QTime time;
    time.start();

    while (!f()) {
        Automator::wait(10);
        if (timeout > 0 && time.elapsed() > timeout) {
            return false;
        }
    }
    return true;
}

QuickFutureUnitTests::QuickFutureUnitTests(QObject *parent) : QObject(parent)
{

}

void QuickFutureUnitTests::test_QFVariantWrapper()
{
    QFuture<QString> future;
    QVariant v = QVariant::fromValue<QFuture<QString> >(future);

    QVERIFY(future.isFinished());

    QFVariantWrapper<QString> wrapper;
    QVERIFY(wrapper.isFinished(v));


}

void QuickFutureUnitTests::test_QFFuture()
{
    QFFuture wrapper;
    QFuture<QString> future;
    QVariant v = QVariant::fromValue<QFuture<QString> >(future);

    QVERIFY(future.isFinished());
    QVERIFY(wrapper.isFinished(v));
}

void QuickFutureUnitTests::PromiseIsNotInstalled()
{
    QQmlApplicationEngine engine;

    qDebug() << "Excepted error:";

    engine.rootContext()->setContextProperty("Actor", new Actor());

    engine.load(QString(SRCDIR) + "/qmltests/PromiseIsNotInstalled.qml");

    QObject* object = engine.rootObjects().first();
    QVERIFY(object);

    QMetaObject::invokeMethod(object, "test",Qt::DirectConnection);

}

void QuickFutureUnitTests::test_subscribe()
{
    QFuture<bool> bFuture;
    QFuture<int> iFuture;
    QFuture<void> vFuture, vFuture2;

    /* Case 1. QFuture<bool> + int(bool) */

    auto bWorker = [=]() -> bool {
        Automator::wait(50);
        return true;
    };

    auto iCleanupBool = [&](bool value) -> int {
        Q_UNUSED(value);
        return 5;
    };

    bFuture = QtConcurrent::run(bWorker);
    iFuture = QuickFlux::subscribe(bFuture, iCleanupBool, this);
    QCOMPARE(iFuture.isFinished(), false);

    QVERIFY(waitUntil([&](){
        return iFuture.isFinished();
    }, 1000));

    QCOMPARE(iFuture.isFinished(), true);
    QCOMPARE(iFuture.result(), 5);

    /* Case 2: QFuture<bool> + void(bool) */

    bool vCleanupBoolCalled = false;

    auto vCleanupBool = [&](bool value) -> void {
        Q_UNUSED(value);
        vCleanupBoolCalled = true;
    };

    bFuture = QtConcurrent::run(bWorker);
    vFuture = QuickFlux::subscribe(bFuture, vCleanupBool, this);
    QCOMPARE(vFuture.isFinished(), false);

    QVERIFY(waitUntil([&](){
        return vFuture.isFinished();
    }, 1000));

    QCOMPARE(vFuture.isFinished(), true);
    QCOMPARE(vCleanupBoolCalled, true);

    /* Case3: QFuture<void> + void(void) */

    bool vCleanupVoidCalled = false;

    auto vCleanupVoid = [&]() -> void {
        vCleanupVoidCalled = true;
    };

    auto vWorker = []() -> void {
            Automator::wait(50);
    };

    vFuture = QtConcurrent::run(vWorker);
    vFuture2 = QuickFlux::subscribe(vFuture, vCleanupVoid, this);
    QCOMPARE(vFuture2.isFinished(), false);

    QVERIFY(waitUntil([&](){
        return vFuture2.isFinished();
    }, 1000));

    QCOMPARE(vFuture2.isFinished(), true);
    QCOMPARE(vCleanupVoidCalled, true);

    /* Remarks: QFuture<void> + void(bool) */
    /* It is not a valid situation */

    /* Extra case. Depend on a finished future */
    vCleanupVoidCalled = false;
    vFuture = QFuture<void>();
    QCOMPARE(vFuture.isFinished(), true);
    vFuture2 = QuickFlux::subscribe(vFuture, vCleanupVoid, this);
    QCOMPARE(vFuture2.isFinished(), false);

    QVERIFY(waitUntil([&](){
        return vFuture2.isFinished();
    }, 1000));

    QCOMPARE(vFuture2.isFinished(), true);
    QCOMPARE(vCleanupVoidCalled, true);
}
