#include <bootsettings.h>
#include <commandlineparser.h>
#include <quicktheme.h>
#include <saveutils.h>

#include <QApplication>

void BootSettings::apply()
{
    QuickTheme::setTheme(CommandlineParser::projectDirectory());

    // Boot settings
    qputenv("QML_DISABLE_DISK_CACHE", "true");

    // QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL); // For web view tool tips
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); // For devices that devicePixelRatio > 1

    if (SaveUtils::projectScaling(CommandlineParser::projectDirectory()) != "noScaling")
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
}