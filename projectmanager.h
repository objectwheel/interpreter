#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QString>

namespace ProjectManager {
QString projectDirectory();
void init(const QString& argv);
}

#endif // PROJECTMANAGER_H
