#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <projectmanager.h>
#include <broadcastingmanager.h>
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

    static bool locked();
    static void prepare();

    static ApplicationCore* instance();
    static QString appDataLocation();
    static QString modulesPath();
    static QString settingsPath();
    static QString deviceUid();
    static QVariantMap deviceInfo();
    static QString recentProjectUid();
    static void setRecentProjectUid(const QString& uid);

private:
    static ApplicationCore* s_instance;
    QSettings m_settings;
    ProjectManager m_projectManager;
    BroadcastingManager m_broadcastingManager;
    ApplicationWindow* m_applicationWindow;

#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    QuitButton* m_quitButton;
#endif

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H