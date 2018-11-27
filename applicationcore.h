#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <QObject>

class QmlApplication;
class GlobalResources;

class ApplicationCore final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ApplicationCore)

public:
    static void init(QObject* parent);

private:
    explicit ApplicationCore(QObject* parent = nullptr);

private slots:
    void onError(const QString& errorString) const;

private:
    static QmlApplication* s_executionManager;
    static GlobalResources* s_globalResources;
};

#endif // APPLICATIONCORE_H