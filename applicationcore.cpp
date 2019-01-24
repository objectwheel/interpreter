#include <applicationcore.h>
#include <components.h>
#include <commandlineparser.h>
#include <quicktheme.h>
#include <saveutils.h>
#include <qtwebviewfunctions.h>

#include <QGuiApplication>
#include <QDebug>
#include <QTimer>

ApplicationCore::ApplicationCore() : m_globalResources(&CommandlineParser::projectDirectory)
{
    // Initialize application
    QGuiApplication::setOrganizationName("Objectwheel");
    QGuiApplication::setOrganizationDomain("objectwheel.com");
    QGuiApplication::setApplicationName("interpreter");
    QGuiApplication::setApplicationVersion("1.0.0");
    QGuiApplication::setApplicationDisplayName("Objectwheel Interpreter");

    // Initialize Web View
    QtWebView::initialize();

    // Initialize Components
    Components::init();

    // Connections
    QObject::connect(&m_qmlApplication, &QmlApplication::quit,
            QCoreApplication::instance(), &QCoreApplication::quit);
    QObject::connect(&m_qmlApplication, &QmlApplication::exit,
            QCoreApplication::instance(), &QCoreApplication::exit);
    QObject::connect(&m_qmlApplication, &QmlApplication::error, [=] (const QString& errorString) {
        qWarning().noquote() << errorString.trimmed();
        qInstallMessageHandler([] (QtMsgType, const QMessageLogContext&, const QString&) {});
        QTimer::singleShot(0, std::bind(&QCoreApplication::exit, EXIT_FAILURE));
    });
}

void ApplicationCore::run()
{
    m_qmlApplication.run(CommandlineParser::projectDirectory());
}

void ApplicationCore::prepare()
{
    QuickTheme::setTheme(CommandlineParser::projectDirectory());
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); // For devices that devicePixelRatio > 1
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
}