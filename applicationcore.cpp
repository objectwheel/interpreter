#include <applicationcore.h>
#include <components.h>
#include <qmlapplication.h>
#include <commandlineparser.h>

#include <QGuiApplication>
#include <QtWebView>

QmlApplication* ApplicationCore::s_executionManager = nullptr;

ApplicationCore::ApplicationCore(QObject* parent) : QObject(parent)
{
    // Initialize application
    QGuiApplication::setOrganizationName("Objectwheel");
    QGuiApplication::setOrganizationDomain("objectwheel.com");
    QGuiApplication::setApplicationName("interpreter");
    QGuiApplication::setApplicationDisplayName("Objectwheel Interpreter");
    QGuiApplication::setApplicationVersion("1.0.0");

    // Init Components
    Components::init();

    // Initialize Web View
    QtWebView::initialize();

    s_executionManager = new QmlApplication(this);
    connect(s_executionManager, &QmlApplication::error,
            this, &ApplicationCore::onError, Qt::QueuedConnection);
    connect(s_executionManager, &QmlApplication::quit,
            qGuiApp, &QGuiApplication::quit, Qt::QueuedConnection);
    connect(s_executionManager, QOverload<int>::of(&QmlApplication::exit),
            this, [=] (int c) { qGuiApp->exit(c); }, Qt::QueuedConnection);
    s_executionManager->exec(CommandlineParser::projectDirectory());
}

void ApplicationCore::onError(const QString& errorString) const
{
    qWarning().noquote() << errorString.trimmed();
    QGuiApplication::exit(EXIT_FAILURE);
}

void ApplicationCore::init(QObject* parent)
{
    static ApplicationCore* instance = nullptr;
    if (instance)
        return;

    instance = new ApplicationCore(parent);
}
