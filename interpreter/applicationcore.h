#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <cleanexit.h>

class QString;
class QmlApplication;

class ApplicationCore final
{
public:
    ApplicationCore();
    ~ApplicationCore();

    static void prepare();

    static QString appDataPath();
    static QString modulesPath();
    static QString projectPath();
    static QString projectResourcePath();

private:
    CleanExit m_cleanExit;
    QmlApplication* m_qmlApplication;

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H