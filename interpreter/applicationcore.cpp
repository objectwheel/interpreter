#include <applicationcore.h>
#include <quicktheme.h>
#include <utilityfunctions.h>
#include <saveutils.h>
#include <qmlapplication.h>
#include <signalwatcher.h>

#include <QGuiApplication>
#include <QStandardPaths>
#include <QFont>
#include <QTimer>
#include <QResource>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

#if defined(Q_OS_ANDROID)
#include <QtAndroid>
#endif

#if defined(QT_WEBVIEW_LIB)
#include <QtWebView>
#endif

ApplicationCore::ApplicationCore() : m_qmlApplication(new QmlApplication(projectPath()))
{
    /** Core initialization **/
    // TODO: QGuiApplication::setWindowIcon(QIcon(":/images/icon.png"));
    QGuiApplication::setFont(UtilityFunctions::systemDefaultFont());

    QObject::connect(SignalWatcher::instance(), &SignalWatcher::signal,
                     QCoreApplication::instance(), [] (int signal) {
        fputs(qPrintable(QStringLiteral("Quit the application by signal(%1)\n").arg(QString::number(signal))), stderr);
        QCoreApplication::exit(EXIT_FAILURE);
    });

    QObject::connect(m_qmlApplication, &QmlApplication::quit,
                     QGuiApplication::instance(), &QGuiApplication::quit, Qt::QueuedConnection);
    QObject::connect(m_qmlApplication, &QmlApplication::exit,
                     QGuiApplication::instance(), &QGuiApplication::exit, Qt::QueuedConnection);
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
    QResource::registerResource(projectResourcePath(), projectPath());

    // FIXME: NEED THIS?
    // QGuiApplication::setAttribute(Qt::AA_UseSoftwareOpenGL); // For webview tooltips

    QuickTheme::setTheme(projectPath());
    if (SaveUtils::projectHdpiScaling(projectPath())) {
        QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    }

#if defined(QT_WEBVIEW_LIB)
    // Not needed on desktop platforms since it
    // is already called by QtWebView::initialize()
    // QtWebEngine::initialize();
    // Also we are calling following before
    // Constructing the QGuiApplication because
    // It uses QtWebEngine as the backend on
    // desktop platforms and it must be initialized
    // before the QGuiApplication constructor
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
    return QFileInfo(QGuiApplication::applicationDirPath() + "/../Frameworks/Modules").canonicalFilePath();
#endif
}

QString ApplicationCore::projectPath()
{
    return QStringLiteral(":/T2JqZWN0d2hlZWxQcm9qZWN0");
}

QString ApplicationCore::projectResourcePath()
{
    // TODO : Think about other versions too
#if defined(Q_OS_ANDROID)
    return QStringLiteral("assets:/Project.rcc");
#elif defined(Q_OS_IOS) || defined(Q_OS_MACOS)
    return QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).last() + QStringLiteral("/Project.rcc");
#else
    return "/Users/omergoktas/Desktop/Project.rcc";
#endif
}
