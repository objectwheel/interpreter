#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <globalresources.h>
#include <discoverymanager.h>
#include <qmlapplication.h>

class ApplicationWindow;

class ApplicationCore final
{
public:
    ApplicationCore();
    ~ApplicationCore();

    static void prepare();

private:
    GlobalResources m_globalResources;
    DiscoveryManager m_discoveryManager;
    QmlApplication m_qmlApplication;
    ApplicationWindow* m_applicationWindow;

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H