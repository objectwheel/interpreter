#include <executer.h>
#include <components.h>
#include <filemanager.h>
#include <projectmanager.h>
#include <saveutils.h>

#include <QtWidgets>
#include <QtWebView>

namespace {
void setTheme(const QString& projectDir);
}

int main(int argc, char* argv[])
{
    if (argc > 1)
        setTheme(argv[1]);

    // Boot settings
    qputenv("QML_DISABLE_DISK_CACHE", "true");

    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL); // For web view tool tips
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    if (argc > 1 && SaveUtils::scaling(argv[1]) != "noScaling")
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Initialize application
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("objectwheel-interpreter");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setWindowIcon(QIcon(":/resources/images/owicon.png"));

    /* Set Font */
#if defined(Q_OS_MACOS)
    QFont font(".SF NS Display");
#elif defined(Q_OS_WIN)
    QFont font("Segoe UI");
#else
    QFont font("Open Sans");
#endif

    font.setPixelSize(14);
    font.setStyleStrategy(QFont::PreferAntialias);
    QApplication::setFont(font);

    // Init Components
    Components::init();

    // Initialize Web View
    QtWebView::initialize();

    if (argc > 1)
        ProjectManager::instance()->init(argv[1]);

    // Start
    Executer::instance()->exec();

    return a.exec();
}


namespace {
void setTheme(const QString& projectDir)
{
    const auto& object = SaveUtils::theme(projectDir).toObject();
    const auto& stylev1 = object.value("stylev1").toString();
    const auto& stylev2 = object.value("stylev2").toString();
    const auto& theme = object.value("theme").toString();
    const auto& accent = object.value("accent").toString();
    const auto& primary = object.value("primary").toString();
    const auto& foreground = object.value("foreground").toString();
    const auto& background = object.value("background").toString();

    qputenv("QT_QUICK_CONTROLS_1_STYLE", stylev1.toUtf8().constData());
    qputenv("QT_QUICK_CONTROLS_STYLE", stylev2.toUtf8().constData());

    if (stylev2 == QString("Material")) {
        if (!theme.isEmpty())
            qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", theme.toUtf8().constData());

        if (!accent.isEmpty())
            qputenv("QT_QUICK_CONTROLS_MATERIAL_ACCENT", accent.toUtf8().constData());

        if (!primary.isEmpty())
            qputenv("QT_QUICK_CONTROLS_MATERIAL_PRIMARY", primary.toUtf8().constData());

        if (!foreground.isEmpty())
            qputenv("QT_QUICK_CONTROLS_MATERIAL_FOREGROUND", foreground.toUtf8().constData());

        if (!background.isEmpty())
            qputenv("QT_QUICK_CONTROLS_MATERIAL_BACKGROUND", background.toUtf8().constData());
    } else if (stylev2 == QString("Universal")) {
        if (!theme.isEmpty())
            qputenv("QT_QUICK_CONTROLS_UNIVERSAL_THEME", theme.toUtf8().constData());

        if (!accent.isEmpty())
            qputenv("QT_QUICK_CONTROLS_UNIVERSAL_ACCENT", accent.toUtf8().constData());

        if (!foreground.isEmpty())
            qputenv("QT_QUICK_CONTROLS_UNIVERSAL_FOREGROUND", foreground.toUtf8().constData());

        if (!background.isEmpty())
            qputenv("QT_QUICK_CONTROLS_UNIVERSAL_BACKGROUND", background.toUtf8().constData());
    }
}
}
