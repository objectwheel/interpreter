#ifndef QMLAPPLICATION_H
#define QMLAPPLICATION_H

#include <QQmlEngine>

class QQuickItem;
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

    struct InitInfo {
        // Form paths
        QVector<QString> forms;
        // Form path, Child paths
        QHash<QString, QVector<QString>> children;
    };

public:
    explicit QmlApplication(const QString& projectDirectory, QObject* parent = nullptr);
    explicit QmlApplication(QObject* parent = nullptr);
    ~QmlApplication() override;

    const QString& projectDirectory() const;
    void setProjectDirectory(const QString& projectDirectory);

    QObject* rootObject() const;

public slots:
    void run();

private:
    QQuickItem* guiItem(QObject* object);
    void setInstanceParent(QmlApplication::ControlInstance* instance, QObject* parentObject);
    ControlInstance createInstance(const QString& dir, const ControlInstance& parentInstance);

private:
    QObject* m_rootObject;
    QString m_projectDirectory;
    QMap<QString, ControlInstance> m_instanceTree;
};

#endif // QMLAPPLICATION_H
