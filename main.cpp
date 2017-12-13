#include <QGuiApplication>
#include <fit.h>
#include <components.h>
#include <executer.h>
#include <projectmanager.h>
#include <QQuickStyle>

#if defined(QT_WEBVIEW_LIB)
#include <QtWebView>
#endif

#define PIXEL_SIZE 14
#define REF_DPI 72.0

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Material");
    qputenv("QT_QUICK_CONTROLS_STYLE", "Base");

    // Init Fit
    fit::update(REF_DPI);

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
    Executer::execProject();

    return app.exec();
}














