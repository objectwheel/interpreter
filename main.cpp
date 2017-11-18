#include <QApplication>
#include <fit.h>
#include <components.h>
#include <QtWebView>
#include <executer.h>

#define PIXEL_SIZE 13
#define REF_WIDTH 700
#define REF_HEIGHT 430
#define REF_DPI 127

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Init application settings
    QApplication::setStyle("fusion");
    qputenv("QT_QUICK_CONTROLS_STYLE", "Base");
    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);
    qsrand(QDateTime::currentMSecsSinceEpoch());

    // Init Fit
    Fit::init(REF_WIDTH, REF_HEIGHT, REF_DPI);

    // Init Components
    Components::init();

    // Initialize Web View
    QtWebView::initialize();

    // Add system wide fonts and set default font
    QFont font;
    font.setPixelSize(Fit::fit(PIXEL_SIZE));
    QApplication::setFont(font);

    // Start MainWidget


    return app.exec();
}














