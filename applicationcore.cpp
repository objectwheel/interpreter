#include <applicationcore.h>
#include <components.h>
#include <commandlineparser.h>
#include <quicktheme.h>
#include <saveutils.h>
#include <qtwebviewfunctions.h>

#include <QApplication>
#include <QIcon>
#include <QDebug>
#include <QTimer>

ApplicationCore::ApplicationCore()
    : m_globalResources(&CommandlineParser::projectDirectory)
    , m_qmlApplication(CommandlineParser::projectDirectory())
{
    // Initialize application
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("interpreter");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");
    QApplication::setWindowIcon(QIcon(":/resources/images/owicon.png"));

    // Initialize Web View
    QtWebView::initialize();

    // Initialize Components
    Components::init();

    // Connections
    QObject::connect(&m_qmlApplication, &QmlApplication::quit,
            QCoreApplication::instance(), &QCoreApplication::quit, Qt::QueuedConnection);
    QObject::connect(&m_qmlApplication, &QmlApplication::exit,
            QCoreApplication::instance(), &QCoreApplication::exit, Qt::QueuedConnection);
}

void ApplicationCore::run()
{
    m_qmlApplication.run();
}

void ApplicationCore::prepare()
{
    QuickTheme::setTheme(CommandlineParser::projectDirectory());
    qputenv("QML_DISABLE_DISK_CACHE", "true");
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); // For devices that devicePixelRatio > 1
    if (SaveUtils::projectScaling(CommandlineParser::projectDirectory()) != "noScaling")
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
}