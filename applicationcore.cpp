#include <applicationcore.h>
#include <components.h>
#include <commandlineparser.h>
#include <quicktheme.h>
#include <saveutils.h>

#include <QtWebView>
#include <QtWebEngine>
#include <QApplication>
#include <QIcon>
#include <QTimer>

ApplicationCore::ApplicationCore() : m_qmlApplication(CommandlineParser::projectDirectory())
{    
    QApplication::setWindowIcon(QIcon(":/resources/images/icon.png"));

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
    // Initialize application
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("interpreter");
    QApplication::setApplicationVersion("1.2.0");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");
    QuickTheme::setTheme(CommandlineParser::projectDirectory());

    if (SaveUtils::projectHdpiScaling(CommandlineParser::projectDirectory())) {
        QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }

    qputenv("QML_DISABLE_DISK_CACHE", "true");
    qputenv("QT_QUICK_CONTROLS_CONF",
            SaveUtils::toProjectAssetsDir(CommandlineParser::projectDirectory()).toUtf8());

    // Not needed on desktop platforms since it
    // is already called by QtWebView::initialize()
    // QtWebEngine::initialize();
    // Also we are calling following before
    // Constructing the QApplication because
    // It uses QtWebEngine as the backend on
    // desktop platforms and it must be initialized
    // before the QApplication constructor
    QtWebView::initialize();
}