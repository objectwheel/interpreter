#include <applicationcore.h>
#include <quicktheme.h>
#include <utilityfunctions.h>
#include <saveutils.h>

#include <QApplication>
#include <QStandardPaths>

#if defined(QT_WEBVIEW_LIB)
#include <QtWebView>
#endif

ApplicationCore::ApplicationCore()
{
    /** Core initialization **/
    QApplication::setApplicationDisplayName(APP_NAME);
    QApplication::setWindowIcon(QIcon(":/images/icon.png"));
    QApplication::setFont(UtilityFunctions::systemDefaultFont());
    QApplication::setStartDragDistance(8);

    // Init Components
    // Components::init();

    // Start
    QTimer::singleShot(0, &Executer::exec);
}

void ApplicationCore::prepare()
{
    // Set those here, needed by QStandardPaths
    QApplication::setApplicationName(APP_NAME);
    QApplication::setOrganizationName(APP_CORP);
    QApplication::setApplicationVersion(APP_VER);
    QApplication::setOrganizationDomain(APP_DOMAIN);

    // FIXME: NEED THIS?
    // QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL); // For webview tooltips

    QuickTheme::setTheme(projectPath());
    if (SaveUtils::projectHdpiScaling(projectPath())) {
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    }

#if defined(QT_WEBVIEW_LIB)
    // Not needed on desktop platforms since it
    // is already called by QtWebView::initialize()
    // QtWebEngine::initialize();
    // Also we are calling following before
    // Constructing the QApplication because
    // It uses QtWebEngine as the backend on
    // desktop platforms and it must be initialized
    // before the QApplication constructor
    QtWebView::initialize();
#endif
}

ApplicationCore* ApplicationCore::instance()
{
    return s_instance;
}

QString ApplicationCore::appDataPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString ApplicationCore::modulesPath()
{
    // TODO : Think about unix and windows versions too
#if defined(Q_OS_ANDROID)
    return QStringLiteral("assets:/Modules");
#else
    return QFileInfo(QApplication::applicationDirPath() + "/../Frameworks/Modules").canonicalFilePath();
#endif
}

QString ApplicationCore::projectPath()
{
#if defined(Q_OS_ANDROID)
    return QStringLiteral("assets:");
#elif defined(Q_OS_IOS)
    return QStandardPaths::standardLocations(QStandardPaths::DataLocation).value(0);
#else
    return QCoreApplication::applicationDirPath();
#endif
}
