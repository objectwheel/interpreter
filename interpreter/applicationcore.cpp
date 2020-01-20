#include <applicationcore.h>
#include <quicktheme.h>
#include <qtwebviewfunctions.h>
#include <applicationwindow.h>
#include <crossplatform.h>
#include <hashfactory.h>
#include <centralwidget.h>
#include <progressbar.h>

#include <QApplication>
#include <QJsonObject>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTimer>
#include <QSharedMemory>
#include <QSurfaceFormat>
#include <QDir>

#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
#include <quitbutton.h>
#endif

ApplicationCore* ApplicationCore::s_instance = nullptr;

ApplicationCore::ApplicationCore() : m_settings(ApplicationCore::settingsPath(), QSettings::IniFormat)
{
    s_instance = this;

    /** Core initialization **/
    QApplication::setApplicationDisplayName(QStringLiteral(APP_NAME));
    QApplication::setWindowIcon(QIcon(QStringLiteral(":/images/icon.png")));
    QApplication::setFont(UtilityFunctions::systemDefaultFont());
    QApplication::setStartDragDistance(8);

    m_applicationWindow = new ApplicationWindow;
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    m_quitButton = new QuitButton;
#endif

    QPalette palette(m_applicationWindow->palette());
    palette.setColor(QPalette::Active, QPalette::Text, "#e5e5e5");
    palette.setColor(QPalette::Inactive, QPalette::Text, "#e5e5e5");
    palette.setColor(QPalette::Disabled, QPalette::Text, "#e5e5e5");
    palette.setColor(QPalette::Active, QPalette::WindowText, "#e5e5e5");
    palette.setColor(QPalette::Inactive, QPalette::WindowText, "#e5e5e5");
    palette.setColor(QPalette::Disabled, QPalette::WindowText, "#b0b0b0");
    palette.setColor(QPalette::Active, QPalette::ButtonText, "#e5e5e5");
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, "#e5e5e5");
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, "#b0b0b0");
    palette.setColor(QPalette::Window, "#2F363C");
    palette.setColor(QPalette::Base, "#2F363C");
    m_applicationWindow->setPalette(palette);
    m_applicationWindow->show();

    QObject::connect(qApp, &QApplication::lastWindowClosed,
                     std::bind(&ProjectManager::terminateProject, 0, false, false));
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::internalTermination,
                     m_applicationWindow, &ApplicationWindow::show);
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::internalTermination,
                     m_quitButton, &QuitButton::hide, Qt::QueuedConnection);
#endif
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::internalTermination,
                     &m_projectManager, &ProjectManager::cancelImport);
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::internalTermination,
                     std::bind(&ProjectManager::terminateProject, 0, false, true));
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::terminate,
                     &m_projectManager, &ProjectManager::cancelImport);
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::terminate,
                     std::bind(&ProjectManager::terminateProject, 0, true, false));
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::terminate, m_applicationWindow, [=] {
        if (m_applicationWindow->centralWidget()->progressBar()->isVisible()) {
            BroadcastingManager::sendStartReport({});
            BroadcastingManager::sendFinishReport(0, true);
            m_applicationWindow->centralWidget()->progressBar()->hide();
        }
    });
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::downloadStarted,
                     m_applicationWindow->centralWidget()->progressBar(), &ProgressBar::show);
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::downloadProgress, m_applicationWindow, [=] (int p)
    { m_applicationWindow->centralWidget()->progressBar()->setValue(p * 0.66); });
    QObject::connect(&m_projectManager, &ProjectManager::finished,
                     BroadcastingManager::instance(), &BroadcastingManager::sendFinishReport);
    QObject::connect(&m_projectManager, &ProjectManager::finished,
                     m_applicationWindow, &ApplicationWindow::show);
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    QObject::connect(&m_projectManager, &ProjectManager::finished,
                     m_quitButton, &QuitButton::hide, Qt::QueuedConnection);
#endif
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::execute,
                     ProjectManager::instance(), &ProjectManager::importProject);
    QObject::connect(&m_projectManager, &ProjectManager::importProgress,
                     BroadcastingManager::instance(), &BroadcastingManager::sendProgressReport);
    QObject::connect(&m_projectManager, &ProjectManager::importProgress, m_applicationWindow, [=] (int p) {
        m_applicationWindow->centralWidget()->progressBar()->setValue(67 + p / 3);
        if (p > 90)
            m_applicationWindow->centralWidget()->progressBar()->repaint();
    });
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::disconnected,
                     &m_projectManager, &ProjectManager::cancelImport);
    QObject::connect(&m_broadcastingManager, &BroadcastingManager::disconnected,
                     m_applicationWindow->centralWidget()->progressBar(), &ProgressBar::hide);
    QObject::connect(&m_projectManager, &ProjectManager::readyOutput,
                     BroadcastingManager::instance(), &BroadcastingManager::sendOutputReport);
    QObject::connect(&m_projectManager, &ProjectManager::importError,
                     m_applicationWindow->centralWidget()->progressBar(), &ProgressBar::hide);
    QObject::connect(&m_projectManager, &ProjectManager::importError,
                     BroadcastingManager::instance(), &BroadcastingManager::cleanExecutionCache);
    QObject::connect(&m_projectManager, &ProjectManager::importError,
                      BroadcastingManager::instance(), &BroadcastingManager::sendErrorReport);
    QObject::connect(&m_projectManager, &ProjectManager::imported,
                     BroadcastingManager::instance(), &BroadcastingManager::cleanExecutionCache);
    QObject::connect(&m_projectManager, &ProjectManager::imported,
                     &ApplicationCore::setRecentProjectUid);
    QObject::connect(&m_projectManager, &ProjectManager::imported, [=] (const QString& uid) {
        m_applicationWindow->centralWidget()->progressBar()->hide();
        m_applicationWindow->activateWindow(); // Make qml window activated after the app window hidden
        m_applicationWindow->raise();
        if (m_applicationWindow->mightThemeChange(uid)) {
            BroadcastingManager::sendStartReport({});
            BroadcastingManager::sendFinishReport(0, false);
            QTimer::singleShot(1500, &CrossPlatform::restart);
#if !defined(Q_OS_IOS)
            QMessageBox::information(m_applicationWindow, QObject::tr("Restarting"),
                                     QObject::tr("Restarting in 2 seconds..."));
#endif
            return;
        }
        m_applicationWindow->hide();
        m_projectManager.startProject(uid);
    });
    QObject::connect(&m_projectManager, &ProjectManager::aboutToStart,
                     BroadcastingManager::instance(), &BroadcastingManager::sendStartReport);

#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    QObject::connect(&m_projectManager, &ProjectManager::aboutToStart, m_quitButton, [=] {
#if defined(Q_OS_IOS)
        m_quitButton->setPosition(10, 30);
#else
        m_quitButton->setPosition(10, 10);
#endif
        m_quitButton->show();
        m_quitButton->raise();
    }, Qt::QueuedConnection);

    QObject::connect(m_quitButton, &QuitButton::clicked,
                     m_quitButton, [=] {
        m_quitButton->hide();
        ProjectManager::terminateProject(0, false);
    });
#endif
}

ApplicationCore::~ApplicationCore()
{
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    delete m_quitButton;
#endif
    delete m_applicationWindow;
    s_instance = nullptr;
}

void ApplicationCore::prepare()
{
    // Set those here, needed by QStandardPaths
    QApplication::setApplicationName(QStringLiteral(APP_NAME));
    QApplication::setOrganizationName(QStringLiteral(APP_CORP));
    QApplication::setApplicationVersion(QStringLiteral(APP_VER));
    QApplication::setOrganizationDomain(QStringLiteral(APP_DOMAIN));

    if (!recentProjectUid().isEmpty())
        QuickTheme::setTheme(ProjectManager::projectDirectory(recentProjectUid()));
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

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

ApplicationCore* ApplicationCore::instance()
{
    return s_instance;
}

bool ApplicationCore::locked()
{
#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    QSharedMemory* sharedMemory = new QSharedMemory("T2JqZWN0d2hlZWxJbnRlcnByZXRlclNoYXJlZE1lbW9yeUtleQ");
    if(!sharedMemory->create(1)) {
        sharedMemory->attach();
        sharedMemory->detach();
        if(!sharedMemory->create(1)) {
            UtilityFunctions::showMessage(nullptr,
                                          QObject::tr("Quitting"),
                                          QObject::tr("Another instance is already running."));
            return true;
        }
    }
#endif
    return false;
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

QString ApplicationCore::settingsPath()
{
    return appDataPath() + "/Settings.ini";
}

QString ApplicationCore::deviceUid()
{
    Q_ASSERT(s_instance);
    if (s_instance == 0)
        return QString();
    static QString uidKey(QStringLiteral("DeviceInfo/DeviceUid"));
    static QString uid(s_instance->m_settings.value(uidKey).toString());
    if (uid.isEmpty()) {
        uid = HashFactory::generate();
        s_instance->m_settings.setValue(uidKey, uid);
    }

    return uid;
}

QVariantMap ApplicationCore::deviceInfo()
{
    static const QJsonObject info = {
        {QStringLiteral("currentCpuArchitecture"), QSysInfo::currentCpuArchitecture()},
        {QStringLiteral("kernelType"), QSysInfo::kernelType()},
        {QStringLiteral("kernelVersion"), QSysInfo::kernelVersion()},
        {QStringLiteral("prettyProductName"), QSysInfo::prettyProductName()},
        {QStringLiteral("productType"), QSysInfo::productType()},
        {QStringLiteral("deviceName"), CrossPlatform::deviceName()},
        {QStringLiteral("deviceUid"), deviceUid()},
        {QStringLiteral("version"), QLatin1String(APP_VER)},
        {QStringLiteral("isEmulator"), CrossPlatform::isEmulator()}
    };
    return info.toVariantMap();
}

QString ApplicationCore::recentProjectUid()
{
    const QSettings settings(settingsPath(), QSettings::IniFormat);
    return settings.value(QStringLiteral("DeviceInfo/RecentProjectUid")).toString();
}

void ApplicationCore::setRecentProjectUid(const QString& uid)
{
    Q_ASSERT(s_instance);
    if (s_instance == 0)
        return;
    s_instance->m_settings.setValue(QStringLiteral("DeviceInfo/RecentProjectUid"), uid);
}
