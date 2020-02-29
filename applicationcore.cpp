#include <applicationcore.h>
#include <commandlineparser.h>
#include <quicktheme.h>
#include <saveutils.h>
#include <utilityfunctions.h>
#include <signalwatcher.h>

#include <QtWebView>
#include <QApplication>
#include <QIcon>
#include <QTimer>

ApplicationCore::ApplicationCore() : m_qmlApplication(CommandlineParser::projectDirectory())
{
    /** Core initialization **/
    QApplication::setApplicationName(QStringLiteral(APP_NAME));
    QApplication::setOrganizationName(QStringLiteral(APP_CORP));
    QApplication::setApplicationVersion(QStringLiteral(APP_VER));
    QApplication::setOrganizationDomain(QStringLiteral(APP_DOMAIN));
    QApplication::setApplicationDisplayName(QStringLiteral(APP_NAME) + QObject::tr(" Interpreter"));
    QApplication::setWindowIcon(QIcon(QStringLiteral(":/images/icon.png")));
    QApplication::setFont(UtilityFunctions::systemDefaultFont());

    QObject::connect(SignalWatcher::instance(), &SignalWatcher::signal,
                     QCoreApplication::instance(), [] (int signal) {
        fputs(qPrintable(QStringLiteral("Quit the application by signal(%1)\n").arg(QString::number(signal))), stderr);
        QCoreApplication::exit(EXIT_FAILURE);
    });

    // Connections
    QObject::connect(&m_qmlApplication, &QmlApplication::quit,
                     QCoreApplication::instance(), &QCoreApplication::quit, Qt::QueuedConnection);
    QObject::connect(&m_qmlApplication, &QmlApplication::exit,
                     QCoreApplication::instance(), &QCoreApplication::exit, Qt::QueuedConnection);
}

QString ApplicationCore::modulesPath()
{
    // TODO : Think about unix and windows versions too
    return QFileInfo(QCoreApplication::applicationDirPath() + QStringLiteral("/../Resources/Modules")).canonicalFilePath();
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
    }

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