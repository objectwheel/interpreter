#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>

class ProjectManager final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ProjectManager)

    friend class ApplicationCore;

public:
    static ProjectManager* instance();
    static QString projectPath(const QString& uid);

private:
    explicit ProjectManager(QObject* parent = nullptr);
    ~ProjectManager() override;

private:
    static ApplicationCore* s_instance;
};


#endif // PROJECTMANAGER_H