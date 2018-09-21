#include <bootsettings.h>
#include <commandlineparser.h>
#include <quicktheme.h>
#include <saveutils.h>

#include <QGuiApplication>

void BootSettings::apply()
{
    QuickTheme::setTheme(CommandlineParser::projectDirectory());

    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); // For devices that devicePixelRatio > 1

    if (SaveUtils::projectScaling(CommandlineParser::projectDirectory()) != "noScaling")
        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
}

bool BootSettings::useGuiApplication()
{
    return qEnvironmentVariableIsSet("QT_QUICK_CONTROLS_1_STYLE")
            && qgetenv("QT_QUICK_CONTROLS_1_STYLE") != "Desktop";
}