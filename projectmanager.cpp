#include <projectmanager.h>
#include <zipasync.h>
#include <qmlapplication.h>
#include <saveutils.h>
#include <applicationcore.h>

#include <QFileInfo>
#include <QDir>

ProjectManager* ProjectManager::s_instance = nullptr;
QmlApplication* ProjectManager::s_qmlApplication = nullptr;
QString ProjectManager::s_currentProjectUid;
QFutureWatcher<size_t> ProjectManager::s_zipWatcher;

ProjectManager::ProjectManager(QObject* parent) : QObject(parent)
{
    s_instance = this;

    QObject::connect(&s_zipWatcher, &QFutureWatcherBase::progressValueChanged,
                     this, &ProjectManager::importProgress);

    QObject::connect(&s_zipWatcher, &QFutureWatcherBase::finished, this, [=] {
        if (s_zipWatcher.future().resultCount() > 0) {
            int last = s_zipWatcher.future().resultCount() - 1;
            size_t result = s_zipWatcher.resultAt(last);
            if (result == 0)
                emit importError(tr(s_zipWatcher.progressText().toUtf8().constData()));
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

QString ProjectManager::projectDirectory(const QString& uid)
{
    return ApplicationCore::dataPath() + "/projects/" + uid;
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

    if (QFileInfo::exists(projectDirectory(uid)) && !QDir(projectDirectory(uid)).removeRecursively()) {
        emit instance()->importError(tr("Error removing existing project."));
        return;
    }

    QDir().mkpath(projectDirectory(uid));

    s_zipWatcher.setProperty("__OW_PROJECT_UID__", uid);
    s_zipWatcher.setFuture(ZipAsync::unzip(sourceZipPath, projectDirectory(uid)));

    if (s_zipWatcher.isCanceled()) {
        emit instance()->importError(tr("Cannot extract the project archive for some reason."));
        return;
    }
}

void ProjectManager::cancelImport()
{
    if (s_zipWatcher.isFinished())
        return;

    s_zipWatcher.cancel();
    s_zipWatcher.waitForFinished();

    const QString& uid = s_zipWatcher.property("__OW_PROJECT_UID__").toString();
    if (uid.isEmpty() || !QFileInfo(projectDirectory(uid)).exists())
        return;

    QDir(projectDirectory(uid)).removeRecursively();
}

void ProjectManager::startProject(const QString& uid)
{
    if (s_qmlApplication)
        return;

    s_currentProjectUid = uid;
    qInstallMessageHandler(messageHandler);

    s_qmlApplication = new QmlApplication(projectDirectory(uid));
    QObject::connect(s_qmlApplication, &QmlApplication::quit,
                    std::bind(&ProjectManager::terminateProject, 0, false, false));
    QObject::connect(s_qmlApplication, &QmlApplication::exit,
                     [=] (int retCode) { ProjectManager::terminateProject(retCode, false); });
    emit instance()->aboutToStart(projectDirectory(uid));
    s_qmlApplication->run();
}

void ProjectManager::terminateProject(int retCode, bool crashExit, bool internalTermination)
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
    if (!internalTermination)
        emit instance()->finished(retCode, crashExit);
}

void ProjectManager::messageHandler(QtMsgType, const QMessageLogContext&, const QString& output)
{
    emit instance()->readyOutput(output);
}