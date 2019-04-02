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
    static QString projectDirectory(const QString& uid);

    static void importProject(const QString& uid, const QString& sourceZipPath);
    static void cancelImport();
    static void startProject(const QString& uid);
    static void terminateProject(int retCode, bool crashExit, bool internalTermination = false);

private:
    static void messageHandler(QtMsgType, const QMessageLogContext&, const QString& output);

private:
    explicit ProjectManager(QObject* parent = nullptr);
    ~ProjectManager() override;

signals:
    void importError(const QString& errorString);
    void imported(const QString& uid);
    void importProgress(int progress);

    void aboutToStart(const QString& projectDirectory);
    void readyOutput(const QString& output);
    void finished(int exitCode, bool crashExit);

private:
    static ProjectManager* s_instance;
    static QmlApplication* s_qmlApplication;
    static QString s_currentProjectUid;
    static QFutureWatcher<size_t> s_zipWatcher;
};

#endif // PROJECTMANAGER_H