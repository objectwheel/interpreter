#include <projectmanager.h>
#include <filemanager.h>
#include <zipper.h>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QByteArray>

#define DEFAULT_PROJECT_DIR ("project")

QString ProjectManager::projectDirectory()
{
    QString baseDir;
    #if defined(Q_OS_IOS) || defined(Q_OS_ANDROID) || defined(Q_OS_WINPHONE)
      baseDir = QStandardPaths::standardLocations(QStandardPaths::DataLocation).value(0);
    #else
      baseDir = QCoreApplication::applicationDirPath();
    #endif
    return baseDir + separator() + DEFAULT_PROJECT_DIR;
}

void ProjectManager::initProject()
{
    #if defined(Q_OS_ANDROID)
        auto data = rdfile("assets:/project.zip");
        mkdir(projectDirectory());
        Zipper::extractZip(data, projectDirectory());
    #endif
}
