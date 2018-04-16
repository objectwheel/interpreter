#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>

class ProjectManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ProjectManager)

public:
    static ProjectManager* instance();
    void init(const QString& dir);
    QString projectDirectory();

private:
    ProjectManager() {}

private:
    QString _baseDir;
};

#endif // PROJECTMANAGER_H
