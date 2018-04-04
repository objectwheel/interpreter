#include <executer.h>
#include <components.h>
#include <filemanager.h>
#include <projectmanager.h>
#include <saveutils.h>

#include <QtWidgets>
#include <QtQuickControls2>

#if defined(QT_WEBVIEW_LIB)
#include <QtWebView>
#endif

int main(int argc, char* argv[])
{
    // Boot settings
    qputenv("QT_QUICK_CONTROLS_MATERIAL_FOREGROUND", "#90ffffff");
    qputenv("QT_QUICK_CONTROLS_MATERIAL_ACCENT", "white");
    qputenv("QT_QUICK_CONTROLS_STYLE", "Material"); // Qt Quick Controls 2 Styles [Default, Material, Fusion, Imagine, Universal]
    qputenv("QT_QUICK_CONTROLS_1_STYLE", "Desktop");  // Qt Quick Controls 1 Styles [Base, Flat, Desktop]
    qputenv("QML_DISABLE_DISK_CACHE", "true");
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL); // For web view tool tips
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Initialize application
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("objectwheel-interpreter");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setWindowIcon(QIcon(":/resources/images/owicon.png"));

    // Init Components
    Components::init();

    // Initialize Web View
    #if defined(QT_WEBVIEW_LIB)
    QtWebView::initialize();
    #endif

    if (argc >= 2) {
        ProjectManager::instance()->init(argv[1]);
        QQuickStyle::setStyle(SaveUtils::theme(argv[1]));
        Executer::instance()->init(argv[1]);
    }

    // Start
    Executer::instance()->exec();

    return a.exec();
}
