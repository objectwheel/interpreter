#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <globalresources.h>
#include <discoverymanager.h>
#include <qmlapplication.h>

class ApplicationCore final
{
public:
    ApplicationCore();

    void run();

    static void prepare();
    static bool useGuiApplication();

private:
    GlobalResources m_globalResources;
    DiscoveryManager m_discoveryManager;
    QmlApplication m_qmlApplication;

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H