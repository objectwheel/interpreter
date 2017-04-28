#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWidget>
#include <fit.h>
#include <components.h>
#include <QtWebView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QTimer>
#include <QQmlProperty>
#include <savemanager.h>
#include <QMessageBox>

#define PIXEL_SIZE 13
#define REF_WIDTH 700
#define REF_HEIGHT 430
#define REF_DPI 127

void setupScene(QQuickWidget*);

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
    QQuickWidget w;
    setupScene(&w);

    // Init Save Manager
    SaveManager::init(&w);

    return app.exec();
}

void setupScene(QQuickWidget* w)
{
    w->setMouseTracking(false);
    w->setSource(QUrl("qrc:/resources/qmls/dashboard.qml"));
    w->rootContext()->setContextProperty("dpi", Fit::ratio());

    QTimer::singleShot(0, [=] {
#if !defined(Q_OS_DARWIN)
        w->setSource(QUrl("qrc:/resources/qmls/dashboard.qml"));
#endif
        w->setResizeMode(QQuickWidget::SizeRootObjectToView);
        auto v = QQmlProperty::read(w->rootObject(), "swipeView", w->rootContext());
        auto view = qobject_cast<QQuickItem*>(v.value<QObject*>());
        if (!view) qFatal("Main : Error occurred");
        w->rootContext()->setContextProperty("swipeView", view);
        auto v_2 = qmlContext(view)->contextProperty("page1");
        auto item = qobject_cast<QQuickItem*>(v_2.value<QObject*>());
        if (!item) qFatal("Main : Error occurred");
        w->rootContext()->setContextProperty("page1", item);

# if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID) && !defined(Q_OS_WINPHONE)
        w->resize({REF_WIDTH, REF_HEIGHT});
        Fit::fit(w, Fit::WidthHeight);
        w->show();
# else
        w->showFullScreen();
# endif

        if (!SaveManager::loadDatabase()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Objectwheel Interpreter");
            msgBox.setText("<b>Project is corrupted.</b>");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            qApp->exit(-1);
        }
    });
}














