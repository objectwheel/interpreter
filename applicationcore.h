#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <globalresources.h>
#include <discoverymanager.h>
#include <qmlapplication.h>
#include <QSettings>

class ApplicationWindow;

class ApplicationCore final
{
public:
    ApplicationCore();
    ~ApplicationCore();

    static void prepare();
    static QSettings* settings();
    static QString dataPath();
    static QString deviceUid();
    static QVariantMap deviceInfo();
    static ApplicationCore* instance();

private:
    void startQmlApplication(const QString& projectDirectory);
    void terminateQmlApplication();

private:
    static ApplicationCore* s_instance;
    QSettings m_settings;
    GlobalResources m_globalResources;
    DiscoveryManager m_discoveryManager;
    QmlApplication* m_qmlApplication;
    ApplicationWindow* m_applicationWindow;

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H