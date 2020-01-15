#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <QMap>

class QmlApplication;
class ApplicationCore final
{
    using AppInfo = QMap<QString, QString>;

public:
    ApplicationCore();
    ~ApplicationCore();

    static void prepare();

    static AppInfo appInfo();
    static QString appDataPath();
    static QString modulesPath();
    static QString projectPath();
    static QString projectResourcePath();

private:
    QmlApplication* m_qmlApplication;

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H