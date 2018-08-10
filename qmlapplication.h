#ifndef QMLAPPLICATION_H
#define QMLAPPLICATION_H

#include <QObject>

class QQmlContext;
class QQmlEngine;
class QmlComponent;

class QmlApplication : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QmlApplication)

    struct ControlInstance {
        QString errorString;
        QObject* object;
        QQmlContext* context;
        QmlComponent* component;
    };

public:
    explicit QmlApplication(QObject* parent = nullptr);

public slots:
    void exec(const QString& projectDirectory);

signals:
    void quit();
    void exit(int);
    void error(const QString& errorString);

private:
    ControlInstance createInstance(const QString& dir, const ControlInstance& parentInstance);

private:
    QQmlEngine* m_engine;
};

#endif // QMLAPPLICATION_H
