#include <executer.h>
#include <components.h>
#include <filemanager.h>
#include <projectmanager.h>
#include <saveutils.h>
#include <quicktheme.h>
#include <appfontsettings.h>

#include <QtWidgets>
#include <QtWebView>

int main(int argc, char* argv[])
{
    if (argc > 1)
        QuickTheme::setTheme(argv[1]);

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
    AppFontSettings::apply();

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