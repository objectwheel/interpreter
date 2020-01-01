#include <applicationcore.h>
#include <quicktheme.h>
#include <utilityfunctions.h>
#include <saveutils.h>
#include <qmlapplication.h>

#include <QApplication>
#include <QStandardPaths>
#include <QFont>
#include <QTimer>

#if defined(Q_OS_ANDROID)
#include <QtAndroid>
#endif

#if defined(QT_WEBVIEW_LIB)
#include <QtWebView>
#endif

ApplicationCore::ApplicationCore() : m_qmlApplication(new QmlApplication(projectPath()))
{
    /** Core initialization **/
    QApplication::setApplicationDisplayName(APP_NAME);
    // TODO: QApplication::setWindowIcon(QIcon(":/images/icon.png"));
    QApplication::setFont(UtilityFunctions::systemDefaultFont());
    QApplication::setStartDragDistance(8);

    QObject::connect(m_qmlApplication, &QmlApplication::quit,
                     QApplication::instance(), &QApplication::quit, Qt::QueuedConnection);
    QObject::connect(m_qmlApplication, &QmlApplication::exit,
                     QApplication::instance(), &QApplication::exit, Qt::QueuedConnection);
    m_qmlApplication->run();

#if defined(Q_OS_ANDROID)
    const bool isFullScreen = UtilityFunctions::isAnyChildWindowFullScreen(m_qmlApplication->rootObject());
    QTimer::singleShot(1000, [=] {
        QtAndroid::hideSplashScreen(300);
        if (!isFullScreen)
            QtAndroid::androidActivity().callMethod<void>("setFullScreen", "(Z)V", false);
    });
#endif
}

ApplicationCore::~ApplicationCore()
{
    delete m_qmlApplication;
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
    // TODO : Think about other versions too
#if defined(Q_OS_ANDROID)
    return QStringLiteral("assets:/Project");
#elif defined(Q_OS_IOS)
    return QStandardPaths::standardLocations(QStandardPaths::DataLocation).value(0);
#else
    return QCoreApplication::applicationDirPath();
#endif
}