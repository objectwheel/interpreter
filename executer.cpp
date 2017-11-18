#include <executer.h>
#include <fit.h>
#include <filemanager.h>

#include <QtCore>
#include <QtGui>
#include <QtQml>
#include <QtQuick>

#define SIGN_OWDB "T3dkYl92Mi4w"
#define SIGN_OWCTRL "T3djdHJsX3YyLjA"
#define DIR_THIS "t"
#define DIR_CHILDREN "c"
#define DIR_OWDB "owdb"
#define DIR_MAINFORM "1"
#define DIR_QRC_OWDB ":/resources/qmls/owdb"
#define DIR_QRC_FORM ":/resources/qmls/form"
#define FILE_PROPERTIES "_properties.json"
#define FILE_ICON "icon.png" //TODO: Apply everywhere
#define FILE_MAIN "main.qml" //TODO: Apply everywhere
#define TAG_ID "id"
#define TAG_X "x"
#define TAG_Y "y"
#define TAG_Z "z"
#define TAG_WIDTH "width"
#define TAG_HEIGHT "height"
#define TAG_UID "_uid"
#define TAG_SUID "_suid"
#define TAG_SKIN "_skin"
#define TAG_NAME "_name"
#define TAG_CATEGORY "_category"
#define TAG_OWDB_SIGN "_owdbsign"
#define TAG_OWCTRL_SIGN "_owctrlsign"

using namespace Fit;

enum Type {
    Quick,
    Window,
    NonGui
};

// Returns (only) form paths.
// Returned paths are rootPaths.
QStringList SaveManagerPrivate::formPaths() const
{
    QStringList paths;
    auto projectDir = ProjectManager::projectDirectory(ProjectManager::currentProject());

    if (projectDir.isEmpty())
        return paths;

    auto baseDir = projectDir + separator() + DIR_OWDB;

    for (auto dir : lsdir(baseDir)) {
        auto propertyPath = baseDir + separator() + dir + separator() +
                            DIR_THIS + separator() + FILE_PROPERTIES;
        auto propertyData = rdfile(propertyPath);

        if (isOwctrl(propertyData))
            paths << dname(dname(propertyPath));
    }

    return paths;
}

QStringList childrenPaths(const QString& rootPath, QString suid = QString());

QStringList masterPaths(const QString& topPath);

// Returns true if given path belongs to main form
// It doesn't check whether rootPath belong to a form or not.
bool isMain(const QString& rootPath);

Skin skin(const QString& rootPath);

// Build qml object form url
QObject* requestItem(ExecError& err, QList<QSharedPointer<QQmlComponent>>&,
  const QString& path, QQmlEngine* engine, QQmlContext* context);

// Returns true if the given object is an instance of QQuickItem
Type type(QObject* object);

QString id(const QString& rootPath);

ExecError Executer::execProject()
{
    ExecError error;
    Skin mainSkin = Skin::Invalid;

    for (auto formPath : formPaths()) {
        if (isMain(formPath))
            mainSkin = skin(formPath);
    }

    if (mainSkin == Skin::Invalid) {
        error.type = CommonError;
        return error;
    }

    QList<QObject*> forms;
    QList<QSharedPointer<QQmlComponent>> components;
    QQuickWindow* mainWindow = nullptr;
    QMap<QString, QQmlContext*> formContexes;
    auto engine = new QQmlEngine;

    engine->rootContext()->setContextProperty("dpi", Fit::ratio());
    engine->setOutputWarningsToStandardError(false);

    // Spin for forms (top level masters)
    for (auto formPath : formPaths()) {
        auto _masterPaths = masterPaths(formPath);

        // Spin for masters inside the form (form itself included)
        QMap<QString, QObject*> masterResults;
        for (auto masterPath : _masterPaths) {
            const bool isForm = (_masterPaths.last() == masterPath);
            auto masterContext = new QQmlContext(engine, engine);

            //! Spin for child items of the master
            QMap<QString, QObject*> childResults;
            for (auto childPath : childrenPaths(masterPath)) {
                int index = _masterPaths.indexOf(childPath);
                if (index >= 0) {
                    childResults[childPath] = masterResults[childPath];
                } else {
                    childResults[childPath] = requestItem(error, components,
                      childPath, engine, masterContext);
                    if (childResults[childPath] == nullptr) {
                        engine->deleteLater();
                        return error;
                    }
                    if (type(childResults[childPath]) == Window) {
                        engine->deleteLater();
                        error.type = ChildIsWindowError;
                        return error;
                    }
                }
                masterContext->setContextProperty(id(childPath),
                  childResults[childPath]);
                qApp->processEvents(QEventLoop::AllEvents, 10);
            }

            masterResults[masterPath] = requestItem(error, components,
              masterPath, engine, masterContext);

            qApp->processEvents(QEventLoop::AllEvents, 10);
            if (masterResults[masterPath] == nullptr) {
                engine->deleteLater();
                return error;
            }

            //! Catch this (current spin's) master item
            if (isForm) { // If it's a form (top level master)
                auto form = masterResults[masterPath];
                if (type(form) == NonGui) {
                    engine->deleteLater();
                    error.type = FormIsNonGui;
                    return error;
                }
                if (isMain(masterPath)) {
                    if (!(mainWindow = qobject_cast<QQuickWindow*>(form))) {
                        engine->deleteLater();
                        error.type = MainFormIsntWindowError;
                        return error;
                    }
                }
                forms << form;
                formContexes[masterPath] = masterContext;
            } else { // If it's a "master child inside of the form"
                auto masterItem = masterResults[masterPath];
                if (type(masterItem) == Window) {
                    engine->deleteLater();
                    error.type = ChildIsWindowError;
                    return error;
                }
                if (type(masterItem) == NonGui) {
                    engine->deleteLater();
                    error.type = MasterIsNonGui;
                    return error;
                }
            }
            masterContext->setContextProperty(id(masterPath),
              masterResults[masterPath]);

            //! Place child items into master item visually
            // Only non-master nongui children were passed (because they don't have a visual parent)
            // Others are handled in anyway, either here or above(invalid cases)
            QMap<QString, QObject*> pmap;
            pmap[masterPath] = masterResults[masterPath];
            for (auto result : childResults.keys()) {
                auto pobject = pmap.value(dname(dname(result))); // Master item (a form(master) or a child master)
                if (type(childResults[result]) == NonGui) // Child item (master or non-master, but not form)
                    continue;

                // All childs are quick (not nongui/window)
                // All masters are quick or window (not nongui)

                //NOTE: What if ApplicationWindow's some properties are binding?
                if (type(pobject) == Window) {
                    static_cast<QQuickItem*>(childResults[result])->setParentItem(
                      static_cast<QQuickWindow*>(pobject)->contentItem());
                } else {
                    static_cast<QQuickItem*>(childResults[result])->setParentItem(
                      static_cast<QQuickItem*>(pobject));
                }

                pmap[result] = childResults[result];
                qApp->processEvents(QEventLoop::AllEvents, 10);
            }
        }
    }

    if (mainWindow == nullptr) {
        engine->deleteLater();
        error.type = NoMainForm;
        return error;
    }

    for (auto formPath : formContexes.keys()) {
        for (int i = 0; i < formContexes.keys().size(); i++) { //Don't change 'keys().size()'
            formContexes[formPath]->setContextProperty(
              id(formContexes.keys().at(i)), forms.at(i));
        }
    }

    qApp->processEvents(QEventLoop::AllEvents, 10);

    for (auto comp : components)
        comp->completeCreate();

    return error;
}
