#include <utilityfunctions.h>
#include <QWidget>
#include <QOperatingSystemVersion>

namespace UtilityFunctions {

namespace Internal {

void pushHelper(QDataStream&) {}
void pullHelper(QDataStream&) {}

} // Internal

QWindow* window(const QWidget* widget)
{
    Q_ASSERT(widget);
    QWindow* winHandle = widget->windowHandle();
    if (!winHandle) {
        if (const QWidget* nativeParent = widget->nativeParentWidget())
            winHandle = nativeParent->windowHandle();
    }
    return winHandle;
}

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