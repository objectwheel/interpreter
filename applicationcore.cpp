#include <applicationcore.h>
#include <components.h>
#include <quicktheme.h>
#include <saveutils.h>
#include <qtwebviewfunctions.h>
#include <filemanager.h>
#include <applicationwindow.h>
#include <crossplatform.h>
#include <hashfactory.h>
#include <centralwidget.h>
#include <progressbar.h>

#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QFontDatabase>
#include <QJsonObject>

#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
#include <quitbutton.h>
#endif

ApplicationCore* ApplicationCore::s_instance = nullptr;
ApplicationCore::ApplicationCore()
    : m_globalResources([] { return ProjectManager::projectPath(ProjectManager::currentProjectUid()); })
{
    s_instance = this;

    // Initialize application
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("interpreter");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");

    for (const QString& fontName : lsfile(QLatin1String(":/fonts")))
        QFontDatabase::addApplicationFont(QLatin1String(":/fonts") + separator() + fontName);

#if defined(Q_OS_MACOS)
    QFont font(".SF NS Display");
#elif defined(Q_OS_WIN)
    QFont font("Segoe UI");
#else
    QFont font("Roboto");
#endif
    font.setPixelSize(13);
    font.setStyleStrategy(QFont::PreferAntialias);
    QApplication::setFont(font);

    QPalette palette(QApplication::palette());
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

    QApplication::setPalette(palette);

    // Initialize Web View
    QtWebView::initialize();

    // Initialize Components
    Components::init();

    m_applicationWindow = new ApplicationWindow;
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    m_quitButton = new QuitButton;
#endif
    m_applicationWindow->show();

    QObject::connect(qApp, &QApplication::lastWindowClosed,
                     std::bind(&ProjectManager::terminateProject, 0));
    QObject::connect(&m_discoveryManager, &DiscoveryManager::terminate,
                     std::bind(&ProjectManager::terminateProject, 0));
    QObject::connect(&m_discoveryManager, &DiscoveryManager::downloadStarted,
                     m_applicationWindow->centralWidget()->progressBar(), &ProgressBar::show);
    QObject::connect(&m_discoveryManager, &DiscoveryManager::downloadProgress, m_applicationWindow, [=] (int p)
    { m_applicationWindow->centralWidget()->progressBar()->setValue(p * 0.66); });
    QObject::connect(&m_projectManager, &ProjectManager::finished,
                     DiscoveryManager::instance(), &DiscoveryManager::sendFinishReport);
    QObject::connect(&m_projectManager, &ProjectManager::finished,
                     m_applicationWindow, &ApplicationWindow::show);
    QObject::connect(&m_discoveryManager, &DiscoveryManager::execute,
                     ProjectManager::instance(), &ProjectManager::importProject);
    QObject::connect(&m_projectManager, &ProjectManager::importProgress,
                     DiscoveryManager::instance(), &DiscoveryManager::sendProgressReport);
    QObject::connect(&m_projectManager, &ProjectManager::importProgress, m_applicationWindow, [=] (int p)
    {
        m_applicationWindow->centralWidget()->progressBar()->setValue(67 + p / 3);
        if (p == 100)
            qApp->processEvents();
    });
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
                     m_applicationWindow, &ApplicationWindow::hide);
    QObject::connect(&m_projectManager, &ProjectManager::imported,
                     ProjectManager::instance(), &ProjectManager::startProject);
    QObject::connect(&m_projectManager, &ProjectManager::aboutToStart,
                     m_applicationWindow->centralWidget()->progressBar(), &ProgressBar::hide);
    QObject::connect(&m_projectManager, &ProjectManager::aboutToStart,
                     DiscoveryManager::instance(), &DiscoveryManager::sendStartReport);
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    QObject::connect(&m_projectManager, &ProjectManager::aboutToStart, m_quitButton, [=] {
        m_quitButton->setPosition(10, 10);
        m_quitButton->show();
        m_quitButton->raise();
    });
    QObject::connect(m_quitButton, &QuitButton::clicked,
                     m_quitButton, &QuitButton::hide);
    QObject::connect(m_quitButton, &QuitButton::clicked,
                     std::bind(&ProjectManager::terminateProject, 0));
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
//    QuickTheme::setTheme(CommandlineParser::projectDirectory());
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
}

QSettings* ApplicationCore::settings()
{
    if (s_instance)
        return &s_instance->m_settings;
    return nullptr;
}

QString ApplicationCore::deviceUid()
{
    if (!s_instance)
        return {};

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
        {"deviceUid", deviceUid()}
    };

    return info.toVariantMap();
}

ApplicationCore* ApplicationCore::instance()
{
    return s_instance;
}
