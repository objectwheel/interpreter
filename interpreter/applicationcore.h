#ifndef APPLICATIONCORE_H
#define APPLICATIONCORE_H

class QString;
class ApplicationCore final
{
public:
    ApplicationCore();

    static void prepare();

    static QString appDataPath();
    static QString modulesPath();
    static QString projectPath();

public:
    ApplicationCore(const ApplicationCore&) = delete;
    ApplicationCore& operator=(const ApplicationCore&) = delete;
};

#endif // APPLICATIONCORE_H