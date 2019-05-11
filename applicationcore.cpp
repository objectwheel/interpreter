#include <applicationcore.h>
#include <components.h>
#include <quicktheme.h>
#include <saveutils.h>
#include <qtwebviewfunctions.h>
#include <applicationwindow.h>
#include <crossplatform.h>
#include <hashfactory.h>
#include <centralwidget.h>
#include <progressbar.h>

#include <QApplication>
#include <QFontDatabase>
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
QSettings ApplicationCore::s_settings(ApplicationCore::dataPath() + "/Objectwheel, Inc./interpreter/settings.ini", QSettings::IniFormat); // BUG: FIXME

ApplicationCore::ApplicationCore()
    : m_globalResources([] { return ProjectManager::projectDirectory(ProjectManager::currentProjectUid()); })
{
    s_instance = this;

    for (const QString& fontName : QDir(QLatin1String(":/fonts")).entryList(QDir::Files))
        QFontDatabase::addApplicationFont(QLatin1String(":/fonts/") + fontName);

#if defined(Q_OS_MACOS) || defined(Q_OS_IOS)
    QFont font(".SF NS Display");
#elif defined(Q_OS_WIN)
    QFont font("Segoe UI");
#else
    QFont font("Roboto");
#endif
    font.setPixelSize(13);
    font.setStyleStrategy(QFont::PreferAntialias);
    QApplication::setFont(font);

    QSurfaceFormat format(QSurfaceFormat::defaultFormat());
    format.setAlphaBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);

    // Initialize Web View
    QtWebView::initialize();

    // Initialize Components
    Components::init();

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
    QObject::connect(&m_discoveryManager, &DiscoveryManager::internalTermination,
                     m_applicationWindow, &ApplicationWindow::show);
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    QObject::connect(&m_discoveryManager, &DiscoveryManager::internalTermination,
                     m_quitButton, &QuitButton::hide, Qt::QueuedConnection);
#endif
    QObject::connect(&m_discoveryManager, &DiscoveryManager::internalTermination,
                     &m_projectManager, &ProjectManager::cancelImport);
    QObject::connect(&m_discoveryManager, &DiscoveryManager::internalTermination,
                     std::bind(&ProjectManager::terminateProject, 0, false, true));
    QObject::connect(&m_discoveryManager, &DiscoveryManager::terminate,
                     &m_projectManager, &ProjectManager::cancelImport);
    QObject::connect(&m_discoveryManager, &DiscoveryManager::terminate,
                     std::bind(&ProjectManager::terminateProject, 0, true, false));
    QObject::connect(&m_discoveryManager, &DiscoveryManager::terminate, m_applicationWindow, [=] {
        if (m_applicationWindow->centralWidget()->progressBar()->isVisible()) {
            DiscoveryManager::sendStartReport({});
            DiscoveryManager::sendFinishReport(0, true);
            m_applicationWindow->centralWidget()->progressBar()->hide();
        }
    });
    QObject::connect(&m_discoveryManager, &DiscoveryManager::downloadStarted,
                     m_applicationWindow->centralWidget()->progressBar(), &ProgressBar::show);
    QObject::connect(&m_discoveryManager, &DiscoveryManager::downloadProgress, m_applicationWindow, [=] (int p)
    { m_applicationWindow->centralWidget()->progressBar()->setValue(p * 0.66); });
    QObject::connect(&m_projectManager, &ProjectManager::finished,
                     DiscoveryManager::instance(), &DiscoveryManager::sendFinishReport);
    QObject::connect(&m_projectManager, &ProjectManager::finished,
                     m_applicationWindow, &ApplicationWindow::show);
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    QObject::connect(&m_projectManager, &ProjectManager::finished,
                     m_quitButton, &QuitButton::hide, Qt::QueuedConnection);
#endif
    QObject::connect(&m_discoveryManager, &DiscoveryManager::execute,
                     ProjectManager::instance(), &ProjectManager::importProject);
    QObject::connect(&m_projectManager, &ProjectManager::importProgress,
                     DiscoveryManager::instance(), &DiscoveryManager::sendProgressReport);
    QObject::connect(&m_projectManager, &ProjectManager::importProgress, m_applicationWindow, [=] (int p)
    {
        m_applicationWindow->centralWidget()->progressBar()->setValue(67 + p / 3);
        if (p > 90)
            m_applicationWindow->centralWidget()->progressBar()->repaint();
    });
    QObject::connect(&m_discoveryManager, &DiscoveryManager::disconnected,
                     &m_projectManager, &ProjectManager::cancelImport);
    QObject::connect(&m_discoveryManager, &DiscoveryManager::disconnected,
                     m_applicationWindow->centralWidget()->progressBar(), &ProgressBar::hide);
    QObject::connect(&m_projectManager, &ProjectManager::readyOutput,
                     DiscoveryManager::instance(), &DiscoveryManager::sendOutputReport);
    QObject::connect(&m_projectManager, &ProjectManager::importError,
                     m_applicationWindow->centralWidget()->progressBar(), &ProgressBar::hide);
    QObject::connect(&m_projectManager, &ProjectManager::importError,
                     DiscoveryManager::instance(), &DiscoveryManager::cleanExecutionCache);
    QObject::connect(&m_projectManager, &ProjectManager::importError,
                      DiscoveryManager::instance(), &DiscoveryManager::sendErrorReport);
    QObject::connect(&m_projectManager, &ProjectManager::imported,
                     DiscoveryManager::instance(), &DiscoveryManager::cleanExecutionCache);
    QObject::connect(&m_projectManager, &ProjectManager::imported,
                     &ApplicationCore::setRecentProjectUid);
    QObject::connect(&m_projectManager, &ProjectManager::imported, [=] (const QString& uid) {
        m_applicationWindow->centralWidget()->progressBar()->hide();
        m_applicationWindow->activateWindow(); // Make qml window activated after the app window hidden
        m_applicationWindow->raise();
        if (m_applicationWindow->mightThemeChange(uid)) {
            DiscoveryManager::sendStartReport({});
            DiscoveryManager::sendFinishReport(0, false);
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
                     DiscoveryManager::instance(), &DiscoveryManager::sendStartReport);

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
    // Initialize application
    QApplication::setOrganizationName("Objectwheel, Inc.");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("interpreter");
    QApplication::setApplicationVersion("2.9");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");

    if (!recentProjectUid().isEmpty())
        QuickTheme::setTheme(ProjectManager::projectDirectory(recentProjectUid()));
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
}

bool ApplicationCore::locked()
{
#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    QSharedMemory* sharedMemory = new QSharedMemory("T2JqZWN0d2hlZWxJbnRlcnByZXRlclNoYXJlZE1lbW9yeUtleQ");
    if(!sharedMemory->create(1)) {
        sharedMemory->attach();
        sharedMemory->detach();
        if(!sharedMemory->create(1)) {
            QMessageBox::warning(nullptr,
                                 QObject::tr("Quitting"),
                                 QObject::tr("Another instance is already running."));
            return true;
        }
    }
#endif
    return false;
}

QSettings* ApplicationCore::settings()
{
    return &s_settings;
}

QString ApplicationCore::dataPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString ApplicationCore::deviceUid()
{
    static QString uidKey(QStringLiteral("DeviceInfo/DeviceUid"));
    static QString uid(s_settings.value(uidKey).toString());
    if (uid.isEmpty()) {
        uid = HashFactory::generate();
        s_settings.setValue(uidKey, uid);
    }
    return uid;
}

QString ApplicationCore::recentProjectUid()
{
    static QString uidKey(QStringLiteral("DeviceInfo/RecentProjectUid"));
    return s_settings.value(uidKey).toString();
}

void ApplicationCore::setRecentProjectUid(const QString& uid)
{
    static QString uidKey(QStringLiteral("DeviceInfo/RecentProjectUid"));
    s_settings.setValue(uidKey, uid);
}

QVariantMap ApplicationCore::deviceInfo()
{
    if (!s_instance)
        return QVariantMap();

    static const QJsonObject info = {
        {"currentCpuArchitecture", QSysInfo::currentCpuArchitecture()},
        {"kernelType", QSysInfo::kernelType()},
        {"kernelVersion", QSysInfo::kernelVersion()},
        {"prettyProductName", QSysInfo::prettyProductName()},
        {"productType", QSysInfo::productType()},
        {"isEmulator", CrossPlatform::isEmulator()},
        {"deviceName", CrossPlatform::deviceName()},
        {"deviceUid", deviceUid()},
        {"interpreterVersion", "2.9"}
    };

    return info.toVariantMap();
}

ApplicationCore* ApplicationCore::instance()
{
    return s_instance;
}
