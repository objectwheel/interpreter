#include <projectmanager.h>
#include <zipasync.h>

#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>

ProjectManager* ProjectManager::s_instance = nullptr;

ProjectManager::ProjectManager(QObject* parent) : QObject(parent)
{
    s_instance = this;
}

ProjectManager::~ProjectManager()
{
    s_instance = nullptr;
}

ProjectManager* ProjectManager::instance()
{
    return s_instance;
}

ProjectManager::importProject(const QString& uid, const QString& sourceZipPath)
{
    if (uid.isEmpty())
        return;

    if (sourceZipPath.isEmpty())
        return;

    if (QFileInfo::exists(projectPath(uid)) && !QDir(projectPath(uid)).removeRecursively())
        return;

}

QString ProjectManager::projectPath(const QString& uid)
{
    return QStandardPaths::standardLocations(QStandardPaths::AppDataLocation) + '/' + uid;
}
