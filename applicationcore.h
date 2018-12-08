#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <globalresources.h>
#include <qmlapplication.h>

class ApplicationCore final : public QObject
{
    Q_OBJECT

public:
    explicit ApplicationCore(QObject* parent = nullptr);
    static void prepare();
    void run();

private slots:
    void onError(const QString& errorString) const;

private:
    GlobalResources m_globalResources;
    QmlApplication m_qmlApplication;
};

#endif // APPLICATIONCORE_H