#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <qmlapplication.h>
#include <signalhandler.h>

class ApplicationCore final
{
public:
    ApplicationCore();

    static QString modulesPath();

    void run();

    static void prepare();

private:
    SignalHandler m_signalHandler;
    QmlApplication m_qmlApplication;

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H
