#include <applicationcore.h>
#include <commandlineparser.h>
#include <quicktheme.h>
#include <saveutils.h>
#include <utilityfunctions.h>
#include <appconstants.h>

#include <QtWebView>
#include <QApplication>
#include <QIcon>
#include <QTimer>

ApplicationCore::ApplicationCore() : m_qmlApplication(CommandlineParser::projectDirectory())
{
    /** Core initialization **/
    QApplication::setApplicationName(AppConstants::NAME);
    QApplication::setOrganizationName(AppConstants::COMPANY);
    QApplication::setApplicationVersion(AppConstants::VERSION);
    QApplication::setOrganizationDomain(AppConstants::ROOT_DOMAIN);
    QApplication::setApplicationDisplayName(AppConstants::LABEL);
    QApplication::setWindowIcon(QIcon(QStringLiteral(":/images/icon.png")));
    QApplication::setFont(UtilityFunctions::systemDefaultFont());

    // Handle signals
    QObject::connect(&m_signalHandler, &SignalHandler::interrupted,
                     &m_signalHandler, &SignalHandler::exitGracefully);

    // Connections
    QObject::connect(&m_qmlApplication, &QmlApplication::quit,
                     QCoreApplication::instance(), &QCoreApplication::quit, Qt::QueuedConnection);
    QObject::connect(&m_qmlApplication, &QmlApplication::exit,
                     QCoreApplication::instance(), &QCoreApplication::exit, Qt::QueuedConnection);
}

QString ApplicationCore::modulesPath()
{
#if defined(Q_OS_MACOS)
    return QFileInfo(QCoreApplication::applicationDirPath() + QLatin1String("/../Resources/Modules")).canonicalFilePath();
#else
    return QFileInfo(QCoreApplication::applicationDirPath() + "/Modules").canonicalFilePath();
#endif
}

void ApplicationCore::run()
{
    m_qmlApplication.run();
}

void ApplicationCore::prepare()
{
    QuickTheme::setTheme(CommandlineParser::projectDirectory());
    if (SaveUtils::projectHdpiScaling(CommandlineParser::projectDirectory())) {
        QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    }

    qputenv("QT_FORCE_STDERR_LOGGING", "1");
    qputenv("QML_DISABLE_DISK_CACHE", "true");

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
