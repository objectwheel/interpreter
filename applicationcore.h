#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

#include <qmlapplication.h>

class ApplicationCore final
{
public:
    ApplicationCore();

    void run();

    static void prepare();

private:
    QmlApplication m_qmlApplication;

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H