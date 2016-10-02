#include <QGuiApplication>
#include <QtQml>
#include "qffuture.h"

static QMap<int, QFVariantWrapperBase*> m_wrappers;

Q_DECLARE_METATYPE(QFuture<QString>)

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
        delete wrapper;
        return;
    }

    m_wrappers[typeId] = wrapper;
}

bool QFFuture::isFinished(const QVariant &future)
{
    if (!m_wrappers.contains(typeId(future))) {
        qWarning() << QString("QFFuture: Can not handle input data type: %1").arg(QMetaType::typeName(future.type()));
        return false;
    }

    QFVariantWrapperBase* wrapper = m_wrappers[typeId(future)];
    return wrapper->isFinished(future);
}

void QFFuture::onFinished(const QVariant &future, QJSValue func)
{
    if (!m_wrappers.contains(typeId(future))) {
        qWarning() << QString("QFFuture: Can not handle input data type: %1").arg(QMetaType::typeName(future.type()));
        return;
    }
    QFVariantWrapperBase* wrapper = m_wrappers[typeId(future)];
    wrapper->onFinished(future, func);
}


static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    QFFuture* object = new QFFuture();

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
}

Q_COREAPP_STARTUP_FUNCTION(init)
