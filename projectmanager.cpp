#include <projectmanager.h>
#include <QStandardPaths>
#include <QCoreApplication>

ProjectManager* ProjectManager::instance()
{
    static ProjectManager instance;
    return &instance;
}

void ProjectManager::init(const QString& dir)
{
    _baseDir = dir;
}

QString ProjectManager::projectDirectory()
{
    if (_baseDir.isEmpty()) {
        #if defined(Q_OS_ANDROID)
        _baseDir = QString("assets:");
        #elif defined(Q_OS_IOS) || defined(Q_OS_WINPHONE)
        _baseDir = QStandardPaths::standardLocations(QStandardPaths::DataLocation).value(0);
        #else
        _baseDir = QCoreApplication::applicationDirPath();
        #endif
    }

    return _baseDir;
}