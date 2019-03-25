#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QFutureWatcher>

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
    static void startProject(const QString& uid);
    static void terminateProject(int retCode = 0);

private:
    static void messageHandler(QtMsgType, const QMessageLogContext&, const QString& output);

private:
    explicit ProjectManager(QObject* parent = nullptr);
    ~ProjectManager() override;

signals:
    void readyOutput(const QString& poutput);
    void importError(const QString& errorString);
    void imported(const QString& uid);
    void importProgress(int progress);
    void finished(int exitCode);
    void aboutToStart();

private:
    static ProjectManager* s_instance;
    static QmlApplication* s_qmlApplication;
    static QString s_currentProjectUid;
    static QFutureWatcher<size_t> s_zipWatcher;
};

#endif // PROJECTMANAGER_H