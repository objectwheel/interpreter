#include <qmlapplication.h>
#include <qmlcomponent.h>
#include <saveutils.h>
#include <components.h>

#include <private/qjsengine_p.h>

#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlProperty>
#include <QCoreApplication>

namespace {

void setId(QQmlContext* context, QObject* object, const QString& id)
{
    if (!id.isEmpty() && context)
        context->setContextProperty(id, object);
}
}

QmlApplication::QmlApplication(QObject* parent) : QObject(parent)
{
    connect(&m_engine, &QQmlEngine::quit, this, &QmlApplication::quit);
    connect(&m_engine, &QQmlEngine::exit, this, &QmlApplication::exit);

    QCoreApplication::instance()->setProperty("__qml_using_qqmlapplicationengine", QVariant(true));
    QJSEnginePrivate::addToDebugServer(&m_engine);
}

QmlApplication::~QmlApplication()
{
    QJSEnginePrivate::removeFromDebugServer(&m_engine);

    for (auto instance : m_instanceTree)
        instance.object->disconnect(&m_engine);
}

void QmlApplication::run(const QString& projectDirectory)
{
    m_engine.addImportPath(SaveUtils::toImportsDir(projectDirectory));
    m_engine.addImportPath(SaveUtils::toGlobalDir(projectDirectory));

    /* Create instances, handle parent-child relationship, set ids, save form instances */
    for (const QString& formPath : SaveUtils::formPaths(projectDirectory)) {
        const ControlInstance& formInstance = createInstance(formPath, ControlInstance());

        if (!formInstance.object) {
            emit error(formInstance.errorString);
            goto error;
        }

        m_instanceTree.insert(formPath, formInstance);

        // TODO: What if a child is a master-control?
        for (const QString& childPath : SaveUtils::childrenPaths(formPath)) {
            const ControlInstance& parentInstance = m_instanceTree.value(SaveUtils::toParentDir(childPath));
            const ControlInstance& childInstance = createInstance(childPath, parentInstance);

            if (!childInstance.object) {
                emit error(childInstance.errorString);
                goto error;
            }

            m_instanceTree.insert(childPath, childInstance);
        }
    }

error:
    for (const ControlInstance& instance : m_instanceTree.values())
        instance.component->completeCreate();
}

QmlApplication::ControlInstance QmlApplication::createInstance(const QString& dir,
                                                               const ControlInstance& parentInstance)
{
    Q_ASSERT_X(SaveUtils::isOwctrl(dir), "createInstance", "Owctrl™ structure is corrupted.");

    const QString& url = SaveUtils::toUrl(dir);
    const QString& id = SaveUtils::id(dir);

    Q_ASSERT(!id.isEmpty());

    ControlInstance instance;
    if (SaveUtils::isForm(dir))
        instance.context = new QQmlContext(&m_engine, &m_engine);
    else
        instance.context = parentInstance.context;

#if defined(Q_OS_ANDROID)
    auto component = new QmlComponent(&m_engine, QUrl(url), &m_engine);
#else
    auto component = new QmlComponent(&m_engine, QUrl::fromUserInput(url), &m_engine);
#endif

    QObject* object = component->beginCreate(instance.context);
    object->setParent(&m_engine);

    if (component->isError()) {
        instance.errorString = component->errorString();
        if (object)
            delete object;
        if (SaveUtils::isForm(dir))
            delete instance.context;
        delete component;
        instance.object = nullptr;
        instance.context = nullptr;
        instance.component = nullptr;
        return instance;
    }

    Q_ASSERT(object);

    if (SaveUtils::isForm(dir))
        setId(m_engine.rootContext(), object, id);
    setId(instance.context, object, id);

    instance.object = object;
    instance.component = component;

    if (!SaveUtils::isForm(dir)) {
        QQmlProperty defaultProperty(parentInstance.object);
        Q_ASSERT(defaultProperty.isValid());
        QQmlListReference childList = defaultProperty.read().value<QQmlListReference>();
        childList.append(instance.object);
    }

    return instance;
}