#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <cleanexit.h>
#include <qmlapplication.h>

class ApplicationCore final
{
public:
    ApplicationCore();

    static QString modulesPath();

    void run();

    static void prepare();

private:
    CleanExit m_cleanExit;
    QmlApplication m_qmlApplication;

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H