#include <projectmanager.h>
#include <zipasync.h>
#include <qmlapplication.h>
#include <saveutils.h>

#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>

ProjectManager* ProjectManager::s_instance = nullptr;
QmlApplication* ProjectManager::s_qmlApplication = nullptr;
QString ProjectManager::s_currentProjectUid;
QFutureWatcher<size_t> ProjectManager::s_zipWatcher;

ProjectManager::ProjectManager(QObject* parent) : QObject(parent)
{
    s_instance = this;

    QObject::connect(&s_zipWatcher, &QFutureWatcherBase::progressValueChanged, this, [=]
    { emit importProgress(s_zipWatcher.progressValue()); });

    QObject::connect(&s_zipWatcher, &QFutureWatcherBase::finished, this, [=]
    {
        if (s_zipWatcher.future().resultCount() > 0) {
            int last = s_zipWatcher.future().resultCount() - 1;
            size_t result = s_zipWatcher.resultAt(last);
            if (result == 0)
                emit importError(s_zipWatcher.progressText());
            else if (!s_zipWatcher.isCanceled())
                emit imported(s_zipWatcher.property("__OW_PROJECT_UID__").toString());
        }
    });
}

ProjectManager::~ProjectManager()
{
    if (s_qmlApplication)
        delete s_qmlApplication;
    s_instance = nullptr;
}

ProjectManager* ProjectManager::instance()
{
    return s_instance;
}

QString ProjectManager::currentProjectUid()
{
    return s_currentProjectUid;
}

QString ProjectManager::projectPath(const QString& uid)
{
    const QStringList& dataPath = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    Q_ASSERT(!dataPath.isEmpty());
    return dataPath.first() + '/' + uid;
}

void ProjectManager::importProject(const QString& uid, const QString& sourceZipPath)
{
    if (uid.isEmpty()) {
        qWarning("WARNING: Uid is empty");
        return;
    }

    if (sourceZipPath.isEmpty()) {
        qWarning("WARNING: Source zip path is empty");
        return;
    }

    if (QFileInfo::exists(projectPath(uid)) && !QDir(projectPath(uid)).removeRecursively()) {
        emit instance()->importError("Error removing existing project.");
        return;
    }

    QDir().mkpath(projectPath(uid));

    s_zipWatcher.setProperty("__OW_PROJECT_UID__", uid);
    s_zipWatcher.setFuture(ZipAsync::unzip(sourceZipPath, projectPath(uid)));

    if (s_zipWatcher.isCanceled()) {
        emit instance()->importError("Cannot extract the project archive for some reason.");
        return;
    }
}

void ProjectManager::startProject(const QString& uid)
{
    if (s_qmlApplication)
        return;

    s_currentProjectUid = uid;
    qInstallMessageHandler(messageHandler);

    s_qmlApplication = new QmlApplication(projectPath(uid));
    QObject::connect(s_qmlApplication, &QmlApplication::quit,
                    std::bind(&ProjectManager::terminateProject, 0));
    QObject::connect(s_qmlApplication, &QmlApplication::exit,
                     &ProjectManager::terminateProject);
    emit instance()->aboutToStart();
    s_qmlApplication->run();
}

void ProjectManager::terminateProject(int retCode)
{
    if (!s_qmlApplication)
        return;

    s_currentProjectUid.clear();
    qInstallMessageHandler(nullptr);

    // Workaround: If we don't do this, the windows on
    // QmlApplication will also emit lastWindowClosed
    // when they are deleted, so this will be recalled
    auto qmlApp = s_qmlApplication;
    s_qmlApplication = nullptr;
    delete qmlApp;
    emit instance()->finished(retCode);
}

void ProjectManager::messageHandler(QtMsgType, const QMessageLogContext&, const QString& output)
{
    emit instance()->readyOutput(output);
}