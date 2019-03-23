#ifndef QMLAPPLICATION_H
#define QMLAPPLICATION_H

#include <QQmlEngine>

class QmlComponent;
class QmlApplication final : public QQmlEngine
{
    Q_OBJECT
    Q_DISABLE_COPY(QmlApplication)

    struct ControlInstance {
        QObject* object = nullptr;
        QQmlContext* context = nullptr;
        QmlComponent* component = nullptr;
    };

public:
    explicit QmlApplication(const QString& projectDirectory, QObject* parent = nullptr);
    explicit QmlApplication(QObject* parent = nullptr);
    ~QmlApplication() override;

    const QString& projectDirectory() const;
    void setProjectDirectory(const QString& projectDirectory);

public slots:
    void run();

private:
    ControlInstance createInstance(const QString& dir, const ControlInstance& parentInstance);

private:
    QObject* m_rootObject;
    QString m_projectDirectory;
    QMap<QString, ControlInstance> m_instanceTree;
};

#endif // QMLAPPLICATION_H
