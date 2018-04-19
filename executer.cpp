#include <executer.h>
#include <filemanager.h>
#include <qmlcomponent.h>
#include <projectmanager.h>
#include <parserutils.h>

#include <QtQml>
#include <QtQuick>

namespace {
enum Type
{
    Quick,
    Window,
    NonGui
};
Type type(const QObject* object);
void setInitialProperties(QQuickItem* item, const QString& url);
QObject* create(const QString& path, QQmlEngine* engine, QQmlContext* context, QList<QmlComponent*>& components);
} // Anonymous Namespace

Executer* Executer::instance()
{
    static Executer instance;
    return &instance;
}

void Executer::exec()
{
    struct Form {
        bool isWindow;
        QString id;
        QObject* object;
        QQmlContext* context;
    };

    auto engine = new QQmlEngine;
    const auto& formPaths = SaveUtils::formPaths(ProjectManager::instance()->projectDirectory());

    QList<Form> forms;
    QList<QmlComponent*> components;

    // Spin inside of form directories
    for (const auto& formPath : formPaths) {
        QHash<QString, QObject*> masterResults;
        const auto& masterPaths = SaveUtils::masterPaths(formPath);

        // Spin for masters inside of the form directory (form itself included)
        for (const auto& masterPath : masterPaths) {
            QObject* result;
            QMap<QString, QObject*> childResults;
            bool isMasterForm = masterPaths.last() == masterPath;
            const auto& childPaths = SaveUtils::childrenPaths(masterPath);
            auto masterContext = new QQmlContext(engine, engine);

            // Create children of the master
            for (const auto& childPath : childPaths) {
                QObject* result;
                int isNormal = !masterPaths.contains(childPath);

                if (isNormal) {
                    result = create(
                        childPath,
                        engine,
                        masterContext,
                        components
                    );

                    if (result == nullptr)
                        return qApp->exit(EXIT_FAILURE);

                    if (type(result) == Window)
                        return qApp->exit(EXIT_FAILURE);
                } else
                    result = masterResults.value(childPath);

                masterContext->setContextProperty(SaveUtils::id(childPath), result);
                childResults[childPath] = result;
            }

            // Create master
            result = create(
                masterPath,
                engine,
                masterContext,
                components
            );

            if (result == nullptr)
                return qApp->exit(EXIT_FAILURE);

            // Handle if the master is a form
            if (isMasterForm) {
                Form form;
                form.id = SaveUtils::id(masterPath);
                form.object = result;
                form.context = masterContext;
                form.isWindow = type(result) == Window;

                if (type(form.object) == NonGui)
                    return qApp->exit(EXIT_FAILURE);

                forms << form;
            } else {
                if (type(result) == Window)
                    return qApp->exit(EXIT_FAILURE);

                if (type(result) == NonGui)
                    return qApp->exit(EXIT_FAILURE);
            }

            // Place children of the master visually
            QHash<QString, QObject*> pmap; // Only non-master nongui children were passed (because they don't have a visual parent)
            pmap[masterPath] = result;    // Others are handled in anyway, either here or above(invalid cases)
            for (const auto& path : childResults.keys()) { // WARNING: Based on QMap's ordering feature
                auto pobject = pmap.value(dname(dname(path))); // Master item (a form(master) or a child master)
                auto cobject = childResults.value(path);

                if (type(cobject) == NonGui)  // Child item (master or non-master, but not form)
                    continue;

                // All childs are quick (not nongui/window)
                // All masters are quick or window (not nongui)

                auto citem = qobject_cast<QQuickItem*>(cobject);

                if (citem == nullptr)
                    return qApp->exit(EXIT_FAILURE);

                //NOTE: What if ApplicationWindow's some properties are binding?
                if (type(pobject) == Window) {
                    auto window = qobject_cast<QQuickWindow*>(pobject)->contentItem();
                    citem->setParentItem(window);
                } else {
                    auto item = qobject_cast<QQuickItem*>(pobject);
                    citem->setParentItem(item);
                }

                pmap[path] = cobject;
            }

            masterContext->setContextProperty(SaveUtils::id(masterPath), result);
            masterResults[masterPath] = result;
        }
    }

    for (const auto& form : forms)
        for (const auto& f : forms)
            form.context->setContextProperty(f.id, f.object);

    for (auto component : components)
        component->completeCreate();

    connect(engine, SIGNAL(quit()), qApp, SLOT(quit()));
    connect(engine, SIGNAL(exit(int)), qApp, SLOT(quit()));
}

namespace {
Type type(const QObject* object)
{
    if (object->inherits("QQuickItem"))
        return Quick;

    if (object->isWindowType())
        return Window;

    return NonGui;
}

/*
 * For positioning new dropped controls, and for setting initial
 * size (50x50) of errornous/nongui controls.
 */
void setInitialProperties(QQuickItem* item, const QString& url)
{
    if (!item)
        return;

    if (!ParserUtils::exists(url, "x") && !ParserUtils::contains(url, "anchors."))
        item->setX(SaveUtils::x(dname(dname(url))));

    if (!ParserUtils::exists(url, "y") && !ParserUtils::contains(url, "anchors."))
        item->setY(SaveUtils::y(dname(dname(url))));

    if (!ParserUtils::exists(url, "width") && !ParserUtils::exists(url, "height") && !ParserUtils::contains(url, "anchors."))
        item->setSize(QSizeF(50, 50));
}

// Build qml object form url
QObject* create(const QString& path, QQmlEngine* engine, QQmlContext* context, QList<QmlComponent*>& components)
{
#if defined(Q_OS_ANDROID)
    auto component = new QmlComponent(
        engine,
        QUrl(
            path +
            separator() +
            DIR_THIS +
            separator() +
            "main.qml"
        )
    );
#else
    auto component = new QmlComponent(
        engine,
        QUrl::fromUserInput(
            path +
            separator() +
            DIR_THIS +
            separator() +
            "main.qml"
        )
    );
#endif

    auto object = component->beginCreate(context);

    if (!component->isError() && object != nullptr)
        components << component;
    else
        delete component;

    /* FIXME: But errornous objects can't reach here, they return above?
     * Forms (or their contentItem) have not included here, because;
     * 1. A form cannot be a nongui object
     * 2. An errornous object will be an Quick dummy item,
     *    So, it will already be included
     * 3. A form cannot be a "new dropped control", which
     *    does not need x and y
     */
    setInitialProperties(qobject_cast<QQuickItem*>(object),
        path + separator() + DIR_THIS + separator() + "main.qml");

    return object;
}
} // Anonymous Namespace
