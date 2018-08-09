#ifndef EXECUTIONMANAGER_H
#define EXECUTIONMANAGER_H

#include <QObject>

class ExecutionManager final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ExecutionManager)

public:
    static ExecutionManager* instance();
    static void exec();

private:
    explicit ExecutionManager(QObject* parent = nullptr);
    ~ExecutionManager();

private:
    static ExecutionManager* s_instance;
};

#endif // EXECUTIONMANAGER_H
