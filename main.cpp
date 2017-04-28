#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWidget>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickWidget w;

    // Init application settings
    QApplication::setStyle("fusion");
    qputenv("QT_QUICK_CONTROLS_STYLE", "Base");
    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);
    qsrand(QDateTime::currentMSecsSinceEpoch());

    // Init Fit
    Fit::init(REF_WIDTH, REF_HEIGHT, REF_DPI);

    // Init CSS
    CSS::init();

    // Init Components
    Components::init();

    // Initialize Web View
    QtWebView::initialize();

    // Add system wide fonts and set default font
    QFontDatabase::addApplicationFont(":/resources/fonts/LiberationMono-Regular.ttf");
    QFont font;
    font.setPixelSize(Fit::fit(PIXEL_SIZE));
    QApplication::setFont(font);

    // Start MainWidget
    MainWindow w;
# if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID) && !defined(Q_OS_WINPHONE)
    w.resize({REF_WIDTH, REF_HEIGHT});
    Fit::fit(&w, Fit::WidthHeight);
    w.show();
# else
    w.showFullScreen();
# endif

    return app.exec();
}
