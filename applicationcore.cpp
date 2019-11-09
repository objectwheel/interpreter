#include <applicationcore.h>
#include <commandlineparser.h>
#include <quicktheme.h>
#include <saveutils.h>
#include <utilityfunctions.h>

#include <QtWebView>
#include <QApplication>
#include <QIcon>
#include <QTimer>

ApplicationCore::ApplicationCore() : m_qmlApplication(CommandlineParser::projectDirectory())
{
    /** Core initialization **/
    QApplication::setApplicationName(APP_NAME);
    QApplication::setOrganizationName(APP_CORP);
    QApplication::setApplicationVersion(APP_VER);
    QApplication::setOrganizationDomain(APP_DOMAIN);
    QApplication::setApplicationDisplayName(APP_NAME + QObject::tr(" Interpreter"));
    QApplication::setWindowIcon(QIcon(":/images/icon.png"));

    /* Load default fonts */
    const QString fontPath = ":/fonts";
    for (const QString& fontName : QDir(fontPath).entryList(QDir::Files))
        QFontDatabase::addApplicationFont(fontPath + '/' + fontName);

    /* Set application ui settings */
    QApplication::setFont(UtilityFunctions::defaultFont());
    QApplication::setStyle("fusion");
    QApplication::setStartDragDistance(8);

    // Connections
    QObject::connect(&m_qmlApplication, &QmlApplication::quit,
                     QCoreApplication::instance(), &QCoreApplication::quit, Qt::QueuedConnection);
    QObject::connect(&m_qmlApplication, &QmlApplication::exit,
                     QCoreApplication::instance(), &QCoreApplication::exit, Qt::QueuedConnection);
}

QString ApplicationCore::modulesPath()
{
    // TODO : Think about unix and windows versions too
    return QFileInfo(QApplication::applicationDirPath() + "/../Frameworks/modules").canonicalFilePath();
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