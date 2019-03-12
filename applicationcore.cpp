#include <applicationcore.h>
#include <components.h>
#include <quicktheme.h>
#include <saveutils.h>
#include <qtwebviewfunctions.h>
#include <filemanager.h>
#include <applicationwindow.h>
#include <crossplatform.h>
#include <hashfactory.h>

#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QFontDatabase>
#include <QJsonObject>

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

    QObject::connect(&m_discoveryManager, &DiscoveryManager::terminate,
                     std::bind(&ProjectManager::terminateProject, 0));

    QObject::connect(&m_discoveryManager, &DiscoveryManager::execute,
                     [] (const QString& uid, const QString& projectPath) {
        ProjectManager::importProject(uid, projectPath);
        ProjectManager::startProject(uid);
    });

    m_applicationWindow = new ApplicationWindow;
    m_applicationWindow->show();
}

ApplicationCore::~ApplicationCore()
{
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
        {"buildCpuArchitecture", QSysInfo::buildCpuArchitecture()},
        {"currentCpuArchitecture", QSysInfo::currentCpuArchitecture()},
        {"buildAbi", QSysInfo::buildAbi()},
        {"kernelType", QSysInfo::kernelType()},
        {"kernelVersion", QSysInfo::kernelVersion()},
        {"productType", QSysInfo::productType()},
        {"productVersion", QSysInfo::productVersion()},
        {"prettyProductName", QSysInfo::prettyProductName()},
        {"machineHostName", QSysInfo::machineHostName()},
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
