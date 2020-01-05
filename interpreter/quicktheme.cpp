#include <quicktheme.h>
#include <saveutils.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QQuickStyle>

namespace QuickTheme {

void setTheme(const QString& projectDir, int* version)
{
    const QJsonObject& object = QJsonDocument::fromBinaryData(SaveUtils::projectTheme(projectDir)).object();
    const QString& stylev1 = object.value("stylev1").toString();
    const QString& stylev2 = object.value("stylev2").toString();
    const QString& theme = object.value("theme").toString();
    const QString& accent = object.value("accent").toString();
    const QString& primary = object.value("primary").toString();
    const QString& foreground = object.value("foreground").toString();
    const QString& background = object.value("background").toString();

    qputenv("QT_QUICK_CONTROLS_1_STYLE", stylev1.toUtf8());
    qputenv("QT_LABS_CONTROLS_STYLE", stylev1.toUtf8());
    QQuickStyle::setStyle(stylev2.toUtf8());

    if (stylev2 == "Material") {
        if (!theme.isEmpty())
            qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", theme.toUtf8());

        if (!accent.isEmpty())
            qputenv("QT_QUICK_CONTROLS_MATERIAL_ACCENT", accent.toUtf8());

        if (!primary.isEmpty())
            qputenv("QT_QUICK_CONTROLS_MATERIAL_PRIMARY", primary.toUtf8());

        if (!foreground.isEmpty())
            qputenv("QT_QUICK_CONTROLS_MATERIAL_FOREGROUND", foreground.toUtf8());

        if (!background.isEmpty())
            qputenv("QT_QUICK_CONTROLS_MATERIAL_BACKGROUND", background.toUtf8());
    } else if (stylev2 == "Universal") {
        if (!theme.isEmpty())
            qputenv("QT_QUICK_CONTROLS_UNIVERSAL_THEME", theme.toUtf8());

        if (!accent.isEmpty())
            qputenv("QT_QUICK_CONTROLS_UNIVERSAL_ACCENT", accent.toUtf8());

        if (!foreground.isEmpty())
            qputenv("QT_QUICK_CONTROLS_UNIVERSAL_FOREGROUND", foreground.toUtf8());

        if (!background.isEmpty())
            qputenv("QT_QUICK_CONTROLS_UNIVERSAL_BACKGROUND", background.toUtf8());
    }

    if (version && (!stylev1.isEmpty() || !stylev2.isEmpty()))
        *version = stylev1.isEmpty() ? 2 : 1;
}

bool activeThemeDiffersFrom(const QString& projectDir)
{
    const QJsonObject& object = QJsonDocument::fromBinaryData(SaveUtils::projectTheme(projectDir)).object();
    QString stylev1_1 = object.value("stylev1").toString();
    QString stylev2_1 = object.value("stylev2").toString();
    QString theme_1 = object.value("theme").toString();
    QString accent_1 = object.value("accent").toString();
    QString primary_1 = object.value("primary").toString();
    QString foreground_1 = object.value("foreground").toString();
    QString background_1 = object.value("background").toString();

    QString stylev1_2 = qgetenv("QT_QUICK_CONTROLS_1_STYLE");
    QString stylev2_2 = QQuickStyle::name();

    if (stylev1_1.isEmpty())
        stylev1_1 = "Desktop";
    if (stylev1_2.isEmpty())
        stylev1_2 = "Desktop";
    if (stylev2_1.isEmpty())
        stylev2_1 = "Default";
    if (stylev2_2.isEmpty())
        stylev2_2 = "Default";

    if (stylev1_1 != stylev1_2 || QString::compare(stylev2_1, stylev2_2, Qt::CaseInsensitive) != 0)
        return true;

    QString theme_2;
    QString accent_2;
    QString primary_2;
    QString foreground_2;
    QString background_2;

    if (QString::compare("Material", stylev2_2, Qt::CaseInsensitive) == 0) {
        theme_2 = qgetenv("QT_QUICK_CONTROLS_MATERIAL_THEME");
        accent_2 = qgetenv("QT_QUICK_CONTROLS_MATERIAL_ACCENT");
        primary_2 = qgetenv("QT_QUICK_CONTROLS_MATERIAL_PRIMARY");
        foreground_2 = qgetenv("QT_QUICK_CONTROLS_MATERIAL_FOREGROUND");
        background_2 = qgetenv("QT_QUICK_CONTROLS_MATERIAL_BACKGROUND");
        return theme_1 != theme_2
                || accent_1 != accent_2
                || primary_1 != primary_2
                || foreground_1 != foreground_2
                || background_1 != background_2;
    } else if (QString::compare("Universal", stylev2_2, Qt::CaseInsensitive) == 0) {
        theme_2 = qgetenv("QT_QUICK_CONTROLS_UNIVERSAL_THEME");
        accent_2 = qgetenv("QT_QUICK_CONTROLS_UNIVERSAL_ACCENT");
        foreground_2 = qgetenv("QT_QUICK_CONTROLS_UNIVERSAL_FOREGROUND");
        background_2 = qgetenv("QT_QUICK_CONTROLS_UNIVERSAL_BACKGROUND");
        return theme_1 != theme_2
                || accent_1 != accent_2
                || foreground_1 != foreground_2
                || background_1 != background_2;
    }

    return false;
}

}