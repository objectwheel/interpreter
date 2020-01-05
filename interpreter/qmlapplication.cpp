#include <qmlapplication.h>
#include <qmlcomponent.h>
#include <saveutils.h>
#include <applicationcore.h>

#include <private/qjsengine_p.h>
#include <private/qquickpopup_p.h>

#include <QQmlContext>
#include <QQmlProperty>
#include <QFileInfo>
#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickWindow>

QmlApplication::QmlApplication(const QString& projectDirectory, QObject* parent) : QQmlEngine(parent)
  , m_rootObject(new QObject)
{
    addImportPath(ApplicationCore::modulesPath());
#if defined(Q_OS_ANDROID)
    // Since there is no executable concept in Android
    // C++ apps are compiled and linked as libraries
    // and apps are called by Qt's Android wrapper Java
    // classes via using JNI. First main .so file of
    // the application is loaded and then the code flow
    // is left to the C++ program. So applicationDirPath
    // is where actual libraries are stored on Android.
    // Because our application is also a library in this
    // scenerio. QLibraryInfo doesn't return proper
    // library and plugin locations for some reason, so
    // we use this solution for now.
    addPluginPath(QCoreApplication::applicationDirPath());
#endif

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
    for (const ControlInstance& instance : qAsConst(m_instanceTree))
        instance.object->disconnect(this);
    m_rootObject->disconnect(this);
    delete m_rootObject;
}

const QString& QmlApplication::projectDirectory() const
{
    return m_projectDirectory;
}

void QmlApplication::setProjectDirectory(const QString& projectDirectory)
{
    m_projectDirectory = projectDirectory;
    if (!projectDirectory.isEmpty())
        addImportPath(SaveUtils::toProjectImportsDir(m_projectDirectory));
}

void QmlApplication::run()
{
    if (m_projectDirectory.isEmpty()) {
        qWarning("WARNING: Project directory cannot be empty");
        emit exit(EXIT_FAILURE);
        return;
    }

    if (!QFileInfo::exists(m_projectDirectory)) {
        qWarning("WARNING: Project directory doesn't exist");
        emit exit(EXIT_FAILURE);
        return;
    }

    if (QFileInfo(m_projectDirectory).isFile()) {
        qWarning("WARNING: Project directory cannot point out to a file");
        emit exit(EXIT_FAILURE);
        return;
    }

    // TODO: Apply other Ow™ kinda checks for project consistency

    InitInfo initInfo;
    initInfo.forms = SaveUtils::formPaths(m_projectDirectory);
    for (const QString& formPath : qAsConst(initInfo.forms))
        initInfo.children.insert(formPath, SaveUtils::childrenPaths(formPath));

    // Create instances, handle parent-child relationship, set ids, save form instances
    bool hasErrors = false;
    for (const QString& formPath : qAsConst(initInfo.forms)) {
        const ControlInstance& formInstance = createInstance(formPath, ControlInstance());
        if (!formInstance.object) {
            hasErrors = true;
            continue;
        }

        m_instanceTree.insert(formPath, formInstance);

        foreach (const QString& childPath, initInfo.children.value(formPath)) {
            const ControlInstance& parentInstance = m_instanceTree.value(SaveUtils::toDoubleUp(childPath));
            if (!parentInstance.object)
                continue;

            const ControlInstance& childInstance = createInstance(childPath, parentInstance);

            if (!childInstance.object) {
                hasErrors = true;
                continue;
            }

            m_instanceTree.insert(childPath, childInstance);
        }
    }

    for (const QString& formPath : qAsConst(initInfo.forms)) {
        ControlInstance& formInstance = m_instanceTree[formPath];
        if (formInstance.component) {
            formInstance.component->completeCreate();
            formInstance.component->deleteLater();
            formInstance.component = nullptr;
        }
        foreach (const QString& childPath, initInfo.children.value(formPath)) {
            ControlInstance& childInstance = m_instanceTree[childPath];
            if (childInstance.component) {
                childInstance.component->completeCreate();
                childInstance.component->deleteLater();
                childInstance.component = nullptr;
            }
        }
    }

    if (hasErrors)
        emit exit(EXIT_FAILURE);
}

QQuickItem* QmlApplication::guiItem(QObject* object)
{
    if (!object)
        return nullptr;
    if (object->isWindowType())
        return qobject_cast<QQuickWindow*>(object)->contentItem();
    else if (object->inherits("QQuickPopup"))
        return qobject_cast<QQuickPopup*>(object)->popupItem();
    else
        return qobject_cast<QQuickItem*>(object);
}

void QmlApplication::setInstanceParent(QmlApplication::ControlInstance* instance, QObject* parentObject)
{
    Q_ASSERT(parentObject);
    Q_ASSERT(instance->object);

    if (auto item = qobject_cast<QQuickItem*>(instance->object)) {
        if (item->parentItem())
            item->setParentItem(nullptr);
        item->setParentItem(guiItem(parentObject));
    }

    instance->object->setParent(parentObject);

    QQmlProperty defaultProperty(parentObject);
    if (defaultProperty.isValid()) {
        QQmlListReference childList = defaultProperty.read().value<QQmlListReference>();
        // Q_ASSERT(!qobject_cast<QQuickItem*>(instance->object) || childList.canAppend());
        if (childList.canAppend())
            childList.append(instance->object);
    }
}

QmlApplication::ControlInstance QmlApplication::createInstance(const QString& dir,
                                                               const ControlInstance& parentInstance)
{
    Q_ASSERT_X(SaveUtils::isControlValid(dir), "createInstance", "Owctrl™ structure is corrupted.");

    const QString& url = SaveUtils::toControlMainQmlFile(dir);
    const QString& id = SaveUtils::controlId(dir);

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
        foreach (const QQmlError& error, component->errors())
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

    if (object->isWindowType()) {
        auto window = qobject_cast<QWindow*>(object);
        Q_ASSERT(window);
        connect(window, &QWindow::visibleChanged, this, [=] {
            bool lastWindowClosed = true;
            for (const QWindow* w : QGuiApplication::topLevelWindows()) {
                if (!w->isVisible()
                        || w->transientParent()
                        || w->type() == Qt::ToolTip
                        || w->inherits("QuitButton")
                        || w->inherits("QWidgetWindow"))
                    continue;
                lastWindowClosed = false;
                break;
            }
            if (lastWindowClosed)
                emit quit();
        }, Qt::QueuedConnection);
    }

    if (SaveUtils::isForm(dir))
        rootContext()->setContextProperty(id, object);
    instance.context->setContextProperty(id, object);
    instance.object = object;
    instance.component = component;

    if (!SaveUtils::isForm(dir))
        setInstanceParent(&instance, parentInstance.object);
    else
        instance.object->setParent(m_rootObject);

    return instance;
}

QObject* QmlApplication::rootObject() const
{
    return m_rootObject;
}
