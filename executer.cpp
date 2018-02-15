#include <executer.h>
#include <fit.h>
#include <filemanager.h>
#include <projectmanager.h>

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

#define pS (qApp->primaryScreen())

enum Type {
    Quick,
    Window,
    NonGui
};

QJsonValue property(const QByteArray& propertyData, const QString& property)
{
    if (propertyData.isEmpty())
        return QJsonValue();

    auto jobj = QJsonDocument::fromJson(propertyData).object();
    return jobj[property];
}

bool isOwctrl(const QByteArray& propertyData)
{
    auto sign = property(propertyData, TAG_OWCTRL_SIGN).toString();
    auto uid = property(propertyData, TAG_OWCTRL_SIGN).toString();
    return (sign == SIGN_OWCTRL && !uid.isEmpty());
}

// Returns (only) form paths.
// Returned paths are rootPaths.
QStringList formPaths()
{
    QStringList paths;
    auto projectDir = ProjectManager::projectDirectory();

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

// Returns all children paths (rootPath) within given root path.
// Returns children only if they have match between their and given suid.
// If given suid is empty then rootPath's uid is taken.
QStringList childrenPaths(const QString& rootPath, QString suid = QString())
{
    QStringList paths;

    if (rootPath.isEmpty())
        return paths;

    if (suid.isEmpty()) {
        auto propertyPath = rootPath + separator() + DIR_THIS +
                            separator() + FILE_PROPERTIES;
        auto propertyData = rdfile(propertyPath);
        suid = property(propertyData, TAG_UID).toString();
    }

    auto childrenPath = rootPath + separator() + DIR_CHILDREN;
    for (auto dir : lsdir(childrenPath)) {
        auto propertyPath = childrenPath + separator() + dir + separator() +
                            DIR_THIS + separator() + FILE_PROPERTIES;
        auto propertyData = rdfile(propertyPath);

        if (isOwctrl(propertyData) && property(propertyData, TAG_SUID).toString() == suid) {
            paths << dname(dname(propertyPath));
            paths << childrenPaths(dname(dname(propertyPath)), suid);
        }
    }
    return paths;
}

QStringList controlPaths(const QString& topPath)
{
    QStringList paths;

    if (topPath.isEmpty())
        return paths;

    for (auto path : fps(FILE_PROPERTIES, topPath)) {
        auto propertyData = rdfile(path);
        if (isOwctrl(propertyData))
            paths << dname(dname(path));
    }

    return paths;
}


QString uid(const QString& rootPath)
{
    auto propertyPath = rootPath + separator() + DIR_THIS +
                        separator() + FILE_PROPERTIES;
    auto propertyData = rdfile(propertyPath);
    return property(propertyData, TAG_UID).toString();
}

QString suid(const QString& rootPath)
{
    auto propertyPath = rootPath + separator() + DIR_THIS +
                        separator() + FILE_PROPERTIES;
    auto propertyData = rdfile(propertyPath);
    return property(propertyData, TAG_SUID).toString();
}

qreal x(const QString& rootPath)
{
    auto propertyPath = rootPath + separator() + DIR_THIS +
                        separator() + FILE_PROPERTIES;
    auto propertyData = rdfile(propertyPath);
    return property(propertyData, TAG_X).toDouble();
}

qreal y(const QString& rootPath)
{
    auto propertyPath = rootPath + separator() + DIR_THIS +
                        separator() + FILE_PROPERTIES;
    auto propertyData = rdfile(propertyPath);
    return property(propertyData, TAG_Y).toDouble();
}

qreal z(const QString& rootPath)
{
    auto propertyPath = rootPath + separator() + DIR_THIS +
                        separator() + FILE_PROPERTIES;
    auto propertyData = rdfile(propertyPath);
    return property(propertyData, TAG_Z).toDouble();
}

qreal width(const QString& rootPath)
{
    auto propertyPath = rootPath + separator() + DIR_THIS +
                        separator() + FILE_PROPERTIES;
    auto propertyData = rdfile(propertyPath);
    return property(propertyData, TAG_WIDTH).toDouble();
}

qreal height(const QString& rootPath)
{
    auto propertyPath = rootPath + separator() + DIR_THIS +
                        separator() + FILE_PROPERTIES;
    auto propertyData = rdfile(propertyPath);
    return property(propertyData, TAG_HEIGHT).toDouble();
}

QString id(const QString& rootPath)
{
    auto propertyPath = rootPath + separator() + DIR_THIS +
                        separator() + FILE_PROPERTIES;
    auto propertyData = rdfile(propertyPath);
    return property(propertyData, TAG_ID).toString();
}

bool isForm(const QString& rootPath)
{
    auto projectDir = ProjectManager::projectDirectory();
    auto baseDir = projectDir + separator() + DIR_OWDB;
    return (baseDir == dname(rootPath));
}

QStringList masterPaths(const QString& topPath)
{
    QStringList paths;
    auto controlPaths = ::controlPaths(topPath);

    QStringList foundSuids;
    for (auto path : controlPaths) {
        auto _suid = suid(path);
        if (!_suid.isEmpty() && !foundSuids.contains(_suid))
            foundSuids << _suid;
    }

    for (auto path : controlPaths) {
        if (foundSuids.contains(uid(path)))
            paths << path;
    }

    std::sort(paths.begin(), paths.end(),
              [](const QString& a, const QString& b)
    { return a.size() > b.size(); });

    if (paths.isEmpty() && isForm(topPath))
        paths << topPath;

    return paths;
}

// Returns true if given path belongs to main form
// It doesn't check whether rootPath belong to a form or not.
bool isMain(const QString& rootPath)
{
    return (fname(rootPath) == DIR_MAINFORM);
}

Skin skin(const QString& rootPath)
{
    auto propertyPath = rootPath + separator() + DIR_THIS +
                        separator() + FILE_PROPERTIES;
    auto propertyData = rdfile(propertyPath);
    return Skin(property(propertyData, TAG_SKIN).toInt());
}

Type type(QObject* object)
{
    if (qobject_cast<QQuickItem*>(object) != nullptr)
        return Quick;
    if (object->isWindowType())
        return Window;
    return NonGui;
}

// Build qml object form url
QObject* requestItem(ExecError& err, QList<QSharedPointer<QQmlComponent>>& comps,
  const QString& path, QQmlEngine* engine, QQmlContext* context)
{
#if defined(Q_OS_ANDROID)
    QSharedPointer<QQmlComponent> comp(new QQmlComponent(engine,
      QUrl(path + separator() + DIR_THIS + separator() + "main.qml")));
#else
    QSharedPointer<QQmlComponent> comp(new QQmlComponent(engine,
      QUrl::fromUserInput(path + separator() + DIR_THIS + separator() + "main.qml")));
#endif
    auto item = comp->create(context); // BUG: QTBUG-47633 beginCreate
    if (comp->isError() || !item) {
        err.type = CodeError;
        err.id = id(path);
        err.errors = comp->errors();
    } else {
        comps << comp;
        engine->setObjectOwnership(item, QQmlEngine::JavaScriptOwnership);
        if (type(item) == Window) {
            //            ((QQuickWindow*)item)->setX(x(path)); //FIXME
            //            ((QQuickWindow*)item)->setY(y(path));
            ((QQuickWindow*)item)->setWidth(fit::fx(width(path)));
            ((QQuickWindow*)item)->setHeight(fit::fx(height(path)));
            QRectF r(0,0, ((QQuickWindow*)item)->width(), ((QQuickWindow*)item)->height());
            r.moveCenter(pS->geometry().center());
            ((QQuickWindow*)item)->setX(r.topLeft().x());
            ((QQuickWindow*)item)->setY(r.topLeft().y());
        } else if (type(item) == Quick) {
            ((QQuickItem*)item)->setX(fit::fx(x(path)));
            ((QQuickItem*)item)->setY(fit::fx(y(path)));
            ((QQuickItem*)item)->setWidth(fit::fx(width(path)));
            ((QQuickItem*)item)->setHeight(fit::fx(height(path)));
            ((QQuickItem*)item)->setZ(z(path));
        }
    }
    return item;
}

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

    engine->rootContext()->setContextProperty("dpi", fit::ratio());
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
    } else {
        #if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(Q_OS_WINPHONE)
        mainWindow->showFullScreen();
        QTimer::singleShot(200, [=]{ mainWindow->showMaximized(); });
        #endif
    }

    for (auto formPath : formContexes.keys()) {
        for (int i = 0; i < formContexes.keys().size(); i++) { //Don't change 'keys().size()'
            formContexes[formPath]->setContextProperty(
              id(formContexes.keys().at(i)), forms.at(i));
        }
    }

    qApp->processEvents(QEventLoop::AllEvents, 10);

//    for (auto comp : components)
//        comp->completeCreate();

    return error;
}
