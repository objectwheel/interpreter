#include <executer.h>
#include <fit.h>
#include <filemanager.h>
#include <projectmanager.h>
#include <qmlcomponent.h>
#include <executivewidget.h>

#include <QtWidgets>
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

template<typename QEnum>
static QString estr(QEnum value)
{
  return QString(QMetaEnum::fromType<QEnum>().valueToKey(value));
}

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

Type type(QObject* object)
{
    if (qobject_cast<QQuickItem*>(object) != nullptr)
        return Quick;

    if (object->isWindowType())
        return Window;

    return NonGui;
}

// Build qml object form url
QObject* create(
    const QString& path,
    QQmlEngine* engine,
    QQmlContext* context,
    QList<QmlComponent*>& components
    )
{
    auto component =
    #if defined(Q_OS_ANDROID)
    new QmlComponent(
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
    new QmlComponent(
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

    if (!component->isError() && object != nullptr) {
        const auto t = type(object);

        if (t == Window) {
            auto window = qobject_cast<QQuickWindow*>(object);
            window->setWidth(fit::fx(width(path)));
            window->setHeight(fit::fx(height(path)));
        } else if (t == Quick) {
            auto item = qobject_cast<QQuickItem*>(object);
            item->setX(fit::fx(x(path)));
            item->setY(fit::fx(y(path)));
            item->setWidth(fit::fx(width(path)));
            item->setHeight(fit::fx(height(path)));
            item->setZ(z(path));
        }

        components << component;
    } else
        delete component;

    return object;
}

ExecutiveWidget* Executer::_eW = nullptr;
Executer::Skin Executer::_skin = Executer::Desktop;

void Executer::exec()
{
    struct Form {
        bool isWindow;
        QString id;
        QObject* object;
        QQmlContext* context;
    };

    auto engine = new QQmlEngine;
    const auto& formPaths = ::formPaths();

    engine->rootContext()->setContextProperty("dpi", fit::ratio());

    QList<Form> forms;
    QList<QmlComponent*> components;

    // Spin inside of form directories
    for (const auto& formPath : formPaths) {
        QHash<QString, QObject*> masterResults;
        const auto& masterPaths = ::masterPaths(formPath);

        // Spin for masters inside of the form directory (form itself included)
        for (const auto& masterPath : masterPaths) {
            QObject* result;
            QMap<QString, QObject*> childResults;
            bool isMasterForm = masterPaths.last() == masterPath;
            const auto& childPaths = ::childrenPaths(masterPath);
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

                masterContext->setContextProperty(id(childPath), result);
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
                form.id = id(masterPath);
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

            masterContext->setContextProperty(id(masterPath), result);
            masterResults[masterPath] = result;
        }
    }

    for (const auto& form : forms)
        for (const auto& f : forms)
            form.context->setContextProperty(f.id, f.object);

    for (auto component : components)
        component->completeCreate();

    if (_skin == PhonePortrait || _skin == PhoneLandscape) {
        if (!_eW)
            _eW = new ExecutiveWidget;

        for (const auto& form : forms) {
            if (form.isWindow) {
                _eW->setWindow(qobject_cast<QQuickWindow*>(form.object));
                break;
            }
        }

        _eW->setSkin(_skin);
        _eW->show();
        connect(_eW, SIGNAL(done()), qApp, SLOT(quit()));
    }

    connect(engine, SIGNAL(quit()), qApp, SLOT(quit()));
    connect(engine, SIGNAL(exit(int)), qApp, SLOT(quit()));
}

void Executer::init(const QString& skin)
{
    if (estr(PhoneLandscape) == skin)
        _skin = PhoneLandscape;
    else if (estr(PhonePortrait) == skin)
        _skin = PhonePortrait;
    else
        _skin = Desktop;
}
