#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <projectmanager.h>
#include <globalresources.h>
#include <discoverymanager.h>
#include <QSettings>

#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
class QuitButton;
#endif

class ApplicationWindow;
class ApplicationCore final
{
public:
    ApplicationCore();
    ~ApplicationCore();

    static void prepare();
    static QSettings* settings();
    static QString deviceUid();
    static QString recentProjectUid();
    static void setRecentProjectUid(const QString& uid);
    static QVariantMap deviceInfo();
    static ApplicationCore* instance();

private:
    static ApplicationCore* s_instance;
    static QSettings s_settings;
    ProjectManager m_projectManager;
    GlobalResources m_globalResources;
    DiscoveryManager m_discoveryManager;
    ApplicationWindow* m_applicationWindow;

#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    QuitButton* m_quitButton;
#endif

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H