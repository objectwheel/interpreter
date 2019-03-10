#include <qmlapplication.h>
#include <qmlcomponent.h>
#include <saveutils.h>

#include <private/qjsengine_p.h>

#include <QQmlContext>
#include <QQmlProperty>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDebug>

QmlApplication::QmlApplication(const QString& projectDirectory, QObject* parent) : QQmlEngine(parent)
  , m_rootObject(new QObject)
{
    setProjectDirectory(projectDirectory);
    QCoreApplication::instance()->setProperty("__qml_using_qqmlapplicationengine", QVariant(true));
    QJSEnginePrivate::addToDebugServer(this);
}

QmlApplication::QmlApplication(QObject* parent) : QmlApplication(QString(), parent)
{
}

QmlApplication::~QmlApplication()
{
    QJSEnginePrivate::removeFromDebugServer(this);
    for (auto instance : m_instanceTree)
        instance.object->disconnect(this);
    delete m_rootObject;
}

const QString& QmlApplication::projectDirectory() const
{
    return m_projectDirectory;
}

void QmlApplication::setProjectDirectory(const QString& projectDirectory)
{
    m_projectDirectory = projectDirectory;
    if (!projectDirectory.isEmpty()) {
        addImportPath(SaveUtils::toImportsDir(m_projectDirectory));
        addImportPath(SaveUtils::toGlobalDir(m_projectDirectory));
    }
}

void QmlApplication::run()
{
    if (m_projectDirectory.isEmpty()) {
        qWarning("WARNING: Project directory cannot be empty");
        return;
    }

    if (!QFileInfo::exists(m_projectDirectory)) {
        qWarning("WARNING: Project directory doesn't exist");
        return;
    }

    if (QFileInfo(m_projectDirectory).isFile()) {
        qWarning("WARNING: Project directory cannot point out to a file");
        return;
    }

    // TODO: Apply other Ow™ kinda checks for project consistency

    // Create instances, handle parent-child relationship, set ids, save form instances
    for (const QString& formPath : SaveUtils::formPaths(m_projectDirectory)) {
        const ControlInstance& formInstance = createInstance(formPath, ControlInstance());
        if (!formInstance.object)
            continue;
        m_instanceTree.insert(formPath, formInstance);

        for (const QString& childPath : SaveUtils::childrenPaths(formPath)) {
            const ControlInstance& parentInstance = m_instanceTree.value(SaveUtils::toParentDir(childPath));
            const ControlInstance& childInstance = createInstance(childPath, parentInstance);
            if (!childInstance.object)
                continue;
            m_instanceTree.insert(childPath, childInstance);
        }
    }

    for (ControlInstance& instance : m_instanceTree) {
        instance.component->completeCreate();
        instance.component->deleteLater();
        instance.component = nullptr;
    }
}

QmlApplication::ControlInstance QmlApplication::createInstance(const QString& dir,
                                                               const ControlInstance& parentInstance)
{
    Q_ASSERT_X(SaveUtils::isOwctrl(dir), "createInstance", "Owctrl™ structure is corrupted.");

    const QString& url = SaveUtils::toUrl(dir);
    const QString& id = SaveUtils::id(dir);

    Q_ASSERT(!id.isEmpty());

    ControlInstance instance;
#if defined(Q_OS_ANDROID)
    auto component = new QmlComponent(this, QUrl(url), m_rootObject);
#else
    auto component = new QmlComponent(this, QUrl::fromUserInput(url), m_rootObject);
#endif

    if (SaveUtils::isForm(dir))
        instance.context = new QQmlContext(this, m_rootObject);
    else
        instance.context = parentInstance.context;

    QObject* object = component->beginCreate(instance.context);

    if (component->isError()) {
        for (auto error : component->errors())
            qWarning().noquote() << error.toString();
        if (component->isCompletePending())
            component->completeCreate();
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
        rootContext()->setContextProperty(id, object);
    instance.context->setContextProperty(id, object);
    instance.object = object;
    instance.component = component;

    if (!SaveUtils::isForm(dir)) {
        QQmlProperty defaultProperty(parentInstance.object);
        Q_ASSERT(defaultProperty.isValid());
        instance.object->setParent(parentInstance.object);
        QQmlListReference childList = defaultProperty.read().value<QQmlListReference>();
        childList.append(instance.object);
    } else {
        instance.object->setParent(m_rootObject);
    }

    return instance;
}
