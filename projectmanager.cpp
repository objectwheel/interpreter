#include <projectmanager.h>
#include <zipasync.h>
#include <qmlapplication.h>
#include <discoverymanager.h>
#include <saveutils.h>

#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>

ProjectManager* ProjectManager::s_instance = nullptr;
QmlApplication* ProjectManager::s_qmlApplication = nullptr;
QString ProjectManager::s_currentProjectUid;

ProjectManager::ProjectManager(QObject* parent) : QObject(parent)
{
    s_instance = this;
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
    if (uid.isEmpty())
        return;

    if (sourceZipPath.isEmpty())
        return;

    if (QFileInfo::exists(projectPath(uid)) && !QDir(projectPath(uid)).removeRecursively())
        return;

    QDir().mkpath(projectPath(uid));

    ZipAsync::unzipSync(sourceZipPath, projectPath(uid));
}

void ProjectManager::startProject(const QString& projectDirectory)
{
    if (s_qmlApplication)
        return;

    s_currentProjectUid = SaveUtils::projectUid(projectDirectory);
    qInstallMessageHandler(messageHandler);

    s_qmlApplication = new QmlApplication(projectDirectory);
    QObject::connect(s_qmlApplication, &QmlApplication::quit,
                    std::bind(&ProjectManager::terminateProject, 0));
    QObject::connect(s_qmlApplication, &QmlApplication::exit,
                     &ProjectManager::terminateProject);
    DiscoveryManager::sendStartReport();
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
    DiscoveryManager::sendFinishReport(retCode);
}

void ProjectManager::messageHandler(QtMsgType, const QMessageLogContext&, const QString& output)
{
    DiscoveryManager::sendOutputReport(output);
}