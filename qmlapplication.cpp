#include <qmlapplication.h>
#include <qmlcomponent.h>
#include <saveutils.h>
#include <utilityfunctions.h>

namespace {

void setId(QQmlContext* context, QObject* object, const QString& id)
{
    if (!id.isEmpty() && context)
        context->setContextProperty(id, object);
}
}

QmlApplication::QmlApplication(QObject* parent) : QObject(parent)
  , m_engine(new QQmlEngine(this))
{
    connect(m_engine, &QQmlEngine::quit, this, &QmlApplication::quit);
    connect(m_engine, &QQmlEngine::exit, this, &QmlApplication::exit);
}

void QmlApplication::exec(const QString& projectDirectory)
{
    m_engine->addImportPath(SaveUtils::toImportsDir(projectDirectory));
    m_engine->addImportPath(SaveUtils::toGlobalDir(projectDirectory));
    UtilityFunctions::registerGlobalPath(projectDirectory);
    UtilityFunctions::registerOfflineStorage(m_engine);

    /* Create instances, handle parent-child relationship, set ids, save form instances */
    QMap<QString, ControlInstance> instanceTree;
    for (const QString& formPath : SaveUtils::formPaths(projectDirectory)) {
        const ControlInstance& formInstance = createInstance(formPath, ControlInstance());

        if (!formInstance.object) {
            emit error(formInstance.errorString);
            goto error;
        }

        instanceTree.insert(formPath, formInstance);

        // TODO: What if a child is a master-control?
        for (const QString& childPath : SaveUtils::childrenPaths(formPath)) {
            const ControlInstance& parentInstance = instanceTree.value(SaveUtils::toParentDir(childPath));
            const ControlInstance& childInstance = createInstance(childPath, parentInstance);

            if (!childInstance.object) {
                emit error(childInstance.errorString);
                goto error;
            }

            instanceTree.insert(childPath, childInstance);
        }
    }

error:
    for (const ControlInstance& instance : instanceTree.values())
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
        instance.context = new QQmlContext(m_engine, m_engine);
    else
        instance.context = parentInstance.context;

#if defined(Q_OS_ANDROID)
    auto component = new QmlComponent(m_engine, QUrl(url), m_engine);
#else
    auto component = new QmlComponent(m_engine, QUrl::fromUserInput(url), m_engine);
#endif

    QObject* object = component->beginCreate(instance.context);

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
        setId(m_engine->rootContext(), object, id);
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