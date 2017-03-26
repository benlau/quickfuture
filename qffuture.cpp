#include <QGuiApplication>
#include <QtQml>
#include <QQmlComponent>
#include "qffuture.h"

static QMap<int, QFVariantWrapperBase*> m_wrappers;

Q_DECLARE_METATYPE(QFuture<QString>)
Q_DECLARE_METATYPE(QFuture<int>)
Q_DECLARE_METATYPE(QFuture<void>)
Q_DECLARE_METATYPE(QFuture<bool>)
Q_DECLARE_METATYPE(QFuture<qreal>)
Q_DECLARE_METATYPE(QFuture<QByteArray>)
Q_DECLARE_METATYPE(QFuture<QVariant>)
Q_DECLARE_METATYPE(QFuture<QVariantMap>)


static int typeId(const QVariant& v) {
    return v.userType();
}

QFFuture::QFFuture(QObject *parent) : QObject(parent)
{
}

void QFFuture::registerType(int typeId, QFVariantWrapperBase* wrapper)
{
    if (m_wrappers.contains(typeId)) {
        qWarning() << QString("QFFuture::registerType:It is already registered:%1").arg(QMetaType::typeName(typeId));
        return;
    }

    m_wrappers[typeId] = wrapper;
}

QJSEngine *QFFuture::engine() const
{
    return m_engine;
}

void QFFuture::setEngine(QQmlEngine *engine)
{
    m_engine = engine;
    if (m_engine.isNull()) {
        return;
    }

    QString qml = "import QtQuick 2.0\n"
                  "import QuickPromise 1.0\n"
                  "import Future 1.0\n"
                  "QtObject { \n"
                  "function create(future) {\n"
                  "    var promise = Q.promise();\n"
                  "    Future.onFinished(future, function(value) {\n"
                  "        promise.resolve(value);\n"
                  "    });\n"
                  "    return promise;\n"
                  "}\n"
                  "}\n";

    QQmlComponent comp (engine);
    comp.setData(qml.toUtf8(), QUrl());
    QObject* holder = comp.create();
    if (holder == 0) {
        return;
    }

    promiseCreator = engine->newQObject(holder);
}

bool QFFuture::isFinished(const QVariant &future)
{
    if (!m_wrappers.contains(typeId(future))) {
        qWarning() << QString("Future: Can not handle input data type: %1").arg(QMetaType::typeName(future.type()));
        return false;
    }

    QFVariantWrapperBase* wrapper = m_wrappers[typeId(future)];
    return wrapper->isFinished(future);
}

bool QFFuture::isRunning(const QVariant &future)
{
    if (!m_wrappers.contains(typeId(future))) {
        qWarning() << QString("Future: Can not handle input data type: %1").arg(QMetaType::typeName(future.type()));
        return false;
    }

    QFVariantWrapperBase* wrapper = m_wrappers[typeId(future)];
    return wrapper->isRunning(future);
}

bool QFFuture::isCanceled(const QVariant &future)
{
    if (!m_wrappers.contains(typeId(future))) {
        qWarning() << QString("Future: Can not handle input data type: %1").arg(QMetaType::typeName(future.type()));
        return false;
    }

    QFVariantWrapperBase* wrapper = m_wrappers[typeId(future)];
    return wrapper->isCanceled(future);
}

void QFFuture::onFinished(const QVariant &future, QJSValue func)
{
    if (!m_wrappers.contains(typeId(future))) {
        qWarning() << QString("Future: Can not handle input data type: %1").arg(QMetaType::typeName(future.type()));
        return;
    }
    QFVariantWrapperBase* wrapper = m_wrappers[typeId(future)];
    wrapper->onFinished(m_engine, future, func);
}

void QFFuture::onCanceled(const QVariant &future, QJSValue func)
{
    if (!m_wrappers.contains(typeId(future))) {
        qWarning() << QString("Future: Can not handle input data type: %1").arg(QMetaType::typeName(future.type()));
        return;
    }
    QFVariantWrapperBase* wrapper = m_wrappers[typeId(future)];
    wrapper->onCanceled(m_engine, future, func);
}

QVariant QFFuture::result(const QVariant &future)
{
    QVariant res;
    if (!m_wrappers.contains(typeId(future))) {
        qWarning() << QString("Future: Can not handle input data type: %1").arg(QMetaType::typeName(future.type()));
        return res;
    }

    QFVariantWrapperBase* wrapper = m_wrappers[typeId(future)];
    return wrapper->result(future);
}

QJSValue QFFuture::promise(QJSValue future)
{
    QJSValue create = promiseCreator.property("create");
    QJSValueList args;
    args << future;

    QJSValue result = create.call(args);
    if (result.isError()) {
        qWarning() << "Future.promise: QuickPromise is not installed or setup properly";
        result = QJSValue();
    }

    return result;
}

void QFFuture::sync(const QVariant &future, const QString &propertyInFuture, QObject *target, const QString &propertyInTarget)
{
    if (!m_wrappers.contains(typeId(future))) {
        qWarning() << QString("Future: Can not handle input data type: %1").arg(QMetaType::typeName(future.type()));
        return;
    }


    QFVariantWrapperBase* wrapper = m_wrappers[typeId(future)];
    wrapper->sync(future, propertyInFuture, target, propertyInTarget);
}

static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(scriptEngine);

    QFFuture* object = new QFFuture();
    object->setEngine(engine);

    return object;
}

static void init() {
    QCoreApplication* app = QCoreApplication::instance();
    QObject* tmp = new QObject(app);

    QObject::connect(tmp,&QObject::destroyed,[=]() {
        auto iter = m_wrappers.begin();
        while (iter != m_wrappers.end()) {
            delete iter.value();
            iter++;
        }
    });

    qmlRegisterSingletonType<QFFuture>("Future", 1, 0, "Future", provider);

    QFFuture::registerType<QString>();
    QFFuture::registerType<int>();
    QFFuture::registerType<void>();
    QFFuture::registerType<bool>();
    QFFuture::registerType<qreal>();
    QFFuture::registerType<QByteArray>();
    QFFuture::registerType<QVariant>();
    QFFuture::registerType<QVariantMap>();

}

Q_COREAPP_STARTUP_FUNCTION(init)
