#include <executer.h>
#include <components.h>
#include <filemanager.h>
#include <projectmanager.h>
#include <saveutils.h>
#include <fit.h>

#include <QtWidgets>
#include <QtQuickControls2>

#if defined(QT_WEBVIEW_LIB)
#include <QtWebView>
#endif

#define PIXEL_SIZE 14
#define REF_DPI 149.0

int main(int argc, char* argv[])
{
    // Initialize fit library
    fit::update(REF_DPI);

    // Boot settings
    qputenv("QT_QUICK_CONTROLS_MATERIAL_FOREGROUND", "#90ffffff");
    qputenv("QT_QUICK_CONTROLS_MATERIAL_ACCENT", "white");
    qputenv("QT_QUICK_CONTROLS_STYLE", "Material"); // Qt Quick Controls 2 Styles [Default, Material, Fusion, Imagine, Universal]
    qputenv("QT_QUICK_CONTROLS_1_STYLE", "Desktop");  // Qt Quick Controls 1 Styles [Base, Flat, Desktop]
    qputenv("QML_DISABLE_DISK_CACHE", "true");

    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL); // For web view tool tips
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Initialize application
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("objectwheel-interpreter");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setWindowIcon(QIcon(":/resources/images/owicon.png"));

    QFont font;
    font.setPixelSize(fit::fx(PIXEL_SIZE));
    font.setStyleStrategy(QFont::PreferAntialias);
    #if defined(Q_OS_MACOS)
      font.setFamily(".SF NS Display");
    #elif defined(Q_OS_WIN)
      font.setFamily("Segoe UI");
    #else
      font.setFamily("Open Sans");
    #endif
    QApplication::setFont(font);

    // Init Components
    Components::init();

    // Initialize Web View
    #if defined(QT_WEBVIEW_LIB)
    QtWebView::initialize();
    #endif

    if (argc > 1) {
        ProjectManager::instance()->init(argv[1]);
        QQuickStyle::setStyle(SaveUtils::theme(argv[1]));
    }

    // Start
    Executer::instance()->exec();

    return a.exec();
}
