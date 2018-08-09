#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <QObject>

class ExecutionManager;

class ApplicationCore final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ApplicationCore)

public:
    static void init(QObject* parent);

private:
    explicit ApplicationCore(QObject* parent = nullptr);

private:
    static ExecutionManager* s_executionManager;
};

#endif // APPLICATIONCORE_H