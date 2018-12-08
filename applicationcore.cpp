#include <applicationcore.h>
#include <components.h>
#include <commandlineparser.h>
#include <filemanager.h>
#include <quicktheme.h>
#include <saveutils.h>

#include <QApplication>
#include <QtWebView>

ApplicationCore::ApplicationCore(QObject* parent) : QObject(parent)
  , m_globalResources(&CommandlineParser::projectDirectory, this)
  , m_qmlApplication(this)
{
    // Initialize application
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("interpreter");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setWindowIcon(QIcon(":/resources/images/owicon.png"));

    connect(&m_qmlApplication, &QmlApplication::error,
            this, &ApplicationCore::onError);
    connect(&m_qmlApplication, &QmlApplication::quit,
            QCoreApplication::instance(), &QCoreApplication::quit);
    connect(&m_qmlApplication, &QmlApplication::exit,
            QCoreApplication::instance(), &QCoreApplication::exit);

    // Initialize Web View
    QtWebView::initialize();

    // Initialize Components
    Components::init();
}

void ApplicationCore::prepare()
{
    QuickTheme::setTheme(CommandlineParser::projectDirectory());

    qputenv("QML_DISABLE_DISK_CACHE", "true");

    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); // For devices that devicePixelRatio > 1

    if (SaveUtils::projectScaling(CommandlineParser::projectDirectory()) != "noScaling")
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
}

void ApplicationCore::run()
{
    m_qmlApplication.run(CommandlineParser::projectDirectory());
}

void ApplicationCore::onError(const QString& errorString) const
{
    qWarning().noquote() << errorString.trimmed();
    QCoreApplication::exit(EXIT_FAILURE);
}