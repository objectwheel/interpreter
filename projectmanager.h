#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QString>

namespace ProjectManager
{
    void init(const QString& dir);
    QString projectDirectory();
}

#endif // PROJECTMANAGER_H
