#include <applicationcore.h>
#include <components.h>
#include <commandlineparser.h>
#include <quicktheme.h>
#include <saveutils.h>
#include <qtwebviewfunctions.h>
#include <filemanager.h>
#include <applicationwindow.h>

#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QFontDatabase>

ApplicationCore::ApplicationCore() : m_globalResources(&CommandlineParser::projectDirectory)
{
    // Initialize application
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("interpreter");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");

    for (const QString& fontName : lsfile(QLatin1String(":/fonts")))
        QFontDatabase::addApplicationFont(QLatin1String(":/fonts") + separator() + fontName);

#if defined(Q_OS_MACOS)
    QFont font(".SF NS Display");
#elif defined(Q_OS_WIN)
    QFont font("Segoe UI");
#else
    QFont font("Roboto");
#endif
    font.setPixelSize(13);
    font.setStyleStrategy(QFont::PreferAntialias);
    QApplication::setFont(font);

    QPalette palette(QApplication::palette());
    palette.setColor(QPalette::Active, QPalette::Text, "#ffffff");
    palette.setColor(QPalette::Inactive, QPalette::Text, "#ffffff");
    palette.setColor(QPalette::Disabled, QPalette::Text, "#ffffff");
    palette.setColor(QPalette::Active, QPalette::WindowText, "#ffffff");
    palette.setColor(QPalette::Inactive, QPalette::WindowText, "#ffffff");
    palette.setColor(QPalette::Disabled, QPalette::WindowText, "#b0b0b0");
    palette.setColor(QPalette::Active, QPalette::ButtonText, "#ffffff");
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, "#ffffff");
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, "#b0b0b0");
    palette.setColor(QPalette::Window, "#2F363C");

    QApplication::setPalette(palette);

    // Initialize Web View
    QtWebView::initialize();

    // Initialize Components
    Components::init();

    // Connections
    QObject::connect(&m_qmlApplication, &QmlApplication::quit,
            QApplication::instance(), &QApplication::quit);
    QObject::connect(&m_qmlApplication, &QmlApplication::exit,
            QApplication::instance(), &QApplication::exit);
    QObject::connect(&m_qmlApplication, &QmlApplication::error, [=] (const QString& errorString) {
        qWarning().noquote() << errorString.trimmed();
        qInstallMessageHandler([] (QtMsgType, const QMessageLogContext&, const QString&) {});
        QTimer::singleShot(0, std::bind(&QApplication::exit, EXIT_FAILURE));
    });

    m_applicationWindow = new ApplicationWindow;
    m_applicationWindow->show();
//    m_qmlApplication.run(CommandlineParser::projectDirectory());
}

ApplicationCore::~ApplicationCore()
{
    delete m_applicationWindow;
}

void ApplicationCore::prepare()
{
    QuickTheme::setTheme(CommandlineParser::projectDirectory());
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
}