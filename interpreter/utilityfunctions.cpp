#include <utilityfunctions.h>
#include <QOperatingSystemVersion>

namespace UtilityFunctions {

QFont systemDefaultFont()
{
    QFont font;
#if defined(Q_OS_MACOS)
    if (QOperatingSystemVersion::current() >= QOperatingSystemVersion::MacOSCatalina)
        font.setFamily("SF UI Display");
    else
        font.setFamily(".SF NS Display");
#elif defined(Q_OS_WIN)
    font.setFamily("Segoe UI");
#endif
    font.setPixelSize(13);
    return font;
}

} // UtilityFunctions