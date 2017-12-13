#include <projectmanager.h>
#include <filemanager.h>
#include <zipper.h>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QByteArray>

QString ProjectManager::projectDirectory()
{
    QString baseDir;
    #if defined(Q_OS_ANDROID)
      baseDir = QString("assets:");
    #elif defined(Q_OS_IOS) || defined(Q_OS_WINPHONE)
      baseDir = QStandardPaths::standardLocations(QStandardPaths::DataLocation).value(0);
    #else
      baseDir = QCoreApplication::applicationDirPath();
    #endif
    return baseDir;
}