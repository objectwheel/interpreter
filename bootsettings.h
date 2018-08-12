#ifndef BOOTSETTINGS_H
#define BOOTSETTINGS_H

#include <QtGlobal>

class BootSettings final
{
    Q_DISABLE_COPY(BootSettings)

public:
    static void apply();
    static bool useGuiApplication();

private:
    BootSettings() {}

};

#endif // BOOTSETTINGS_H