#include <QGuiApplication>
#include <fit.h>
#include <components.h>
#include <executer.h>
#include <projectmanager.h>

#if defined(QT_WEBVIEW_LIB)
#include <QtWebView>
#endif

#define PIXEL_SIZE 14
#define REF_WIDTH 1680
#define REF_HEIGHT 1050
#define REF_DPI 72.0

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Init application settings
    qputenv("QT_QUICK_CONTROLS_STYLE", "Base");
    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);
    qsrand(QDateTime::currentMSecsSinceEpoch());

    // Init Fit
    fit::update(REF_WIDTH, REF_HEIGHT, REF_DPI);

    // Init Components
    Components::init();

    // Initialize Web View
    #if defined(QT_WEBVIEW_LIB)
    QtWebView::initialize();
    #endif

    // Add system wide fonts and set default font
    QFont font;
    font.setPixelSize(fit::fx(PIXEL_SIZE));
    QGuiApplication::setFont(font);

    // Start
    ProjectManager::initProject();
    Executer::execProject();

    return app.exec();
}














