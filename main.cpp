#include <fit.h>
#include <executer.h>
#include <components.h>
#include <filemanager.h>
#include <projectmanager.h>

#include <QtWidgets>
#include <QtQuickControls2>

#if defined(QT_WEBVIEW_LIB)
#include <QtWebView>
#endif

#define PIXEL_SIZE 14
#define MIN_DPI 110.0
#define REF_DPI 149.0

int main(int argc, char* argv[])
{
    // Boot settings
    qputenv("QT_QUICK_CONTROLS_STYLE", "Base");
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    // Initialize application
    QGuiApplication a(argc, argv);
    QGuiApplication::setApplicationDisplayName(QObject::tr("Objectwheel Interpreter"));
    QGuiApplication::setWindowIcon(QIcon(":/resources/images/owicon.png"));

    QQuickStyle::setStyle("Material");

    // Multiple instances protection
    QSharedMemory sharedMemory("b2JqZWN0d2hlZWwtaW50ZXJwcmV0ZXI");
    if(!sharedMemory.create(1)) {
        sharedMemory.attach();
        sharedMemory.detach();
        if(!sharedMemory.create(1)) {
            QMessageBox::warning(
                nullptr,
                QObject::tr("Quitting"),
                QObject::tr("Another instance is already running.")
            );
            return EXIT_FAILURE;
        }
    }

    // Initialize fit library
    fit::update(REF_DPI, MIN_DPI);

    // Init Components
    Components::init();

    // Initialize Web View
    #if defined(QT_WEBVIEW_LIB)
    QtWebView::initialize();
    #endif

    // Font settings
    for (const auto& font : lsfile(":/resources/fonts"))
        QFontDatabase::addApplicationFont(":/resources/fonts/" + font);

    // Add system wide fonts and set default font
    QFont font;
    font.setPixelSize(fit::fx(PIXEL_SIZE));
    #if defined(Q_OS_MACOS)
    font.setFamily(".SF NS Display");
    #elif defined(Q_OS_WIN)
    font.setFamily("Segoe UI");
    #else
    font.setFamily("Open Sans");
    #endif
    QGuiApplication::setFont(font);

    if (argc == 2)
        ProjectManager::init(argv[1]);

    // Start
    QTimer::singleShot(0, &Executer::exec);

    return a.exec();
}
