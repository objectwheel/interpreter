#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <projectmanager.h>
#include <globalresources.h>
#include <discoverymanager.h>
#include <QSettings>

class ApplicationWindow;
class QuitButton;

class ApplicationCore final
{
public:
    ApplicationCore();
    ~ApplicationCore();

    static void prepare();
    static QSettings* settings();
    static QString deviceUid();
    static QVariantMap deviceInfo();
    static ApplicationCore* instance();

private:
    static ApplicationCore* s_instance;
    QSettings m_settings;
    ProjectManager m_projectManager;
    GlobalResources m_globalResources;
    DiscoveryManager m_discoveryManager;
    ApplicationWindow* m_applicationWindow;
    QuitButton* m_quitButton;

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H