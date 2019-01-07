#ifndef QMLAPPLICATION_H
#define QMLAPPLICATION_H

#include <QObject>
#include <QQmlEngine>

class QmlComponent;
class QQmlContext;

class QmlApplication final : public QObject
{
    Q_OBJECT

    struct ControlInstance {
        QString errorString;
        QObject* object;
        QQmlContext* context;
        QmlComponent* component;
    };

public:
    explicit QmlApplication(QObject* parent = nullptr);
    ~QmlApplication();

public slots:
    void run(const QString& projectDirectory);

signals:
    void quit();
    void exit(int);
    void error(const QString& errorString);

private:
    ControlInstance createInstance(const QString& dir, const ControlInstance& parentInstance);

private:
    QQmlEngine m_engine;
    QMap<QString, ControlInstance> m_instanceTree;
};

#endif // QMLAPPLICATION_H
