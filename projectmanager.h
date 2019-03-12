#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>

class QmlApplication;

class ProjectManager final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ProjectManager)

    friend class ApplicationCore;

public:
    static ProjectManager* instance();
    static QString currentProjectUid();
    static QString projectPath(const QString& uid);
    static void importProject(const QString& uid, const QString& sourceZipPath);
    static void startProject(const QString& projectDirectory);
    static void terminateProject(int retCode = 0);

private:
    static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

private:
    explicit ProjectManager(QObject* parent = nullptr);
    ~ProjectManager() override;

private:
    static ProjectManager* s_instance;
    static QmlApplication* s_qmlApplication;
    static QString s_currentProjectUid;
};


#endif // PROJECTMANAGER_H