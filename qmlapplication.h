#ifndef QMLAPPLICATION_H
#define QMLAPPLICATION_H

#include <QQmlEngine>

class QmlComponent;
class QQmlContext;

class QmlApplication final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QmlApplication)

    struct ControlInstance
    {
        QString errorString;
        QObject* object;
        QQmlContext* context;
        QmlComponent* component;
    };

public:
    explicit QmlApplication(QObject* parent = nullptr);
    ~QmlApplication() override;

public slots:
    void run(const QString& projectDirectory);

private:
    ControlInstance createInstance(const QString& dir, const ControlInstance& parentInstance);

signals:
    void quit();
    void exit(int);
    void error(const QString& errorString);

private:
    QQmlEngine m_engine;
    QObject m_rootObject;
    QMap<QString, ControlInstance> m_instanceTree;
};

#endif // QMLAPPLICATION_H
