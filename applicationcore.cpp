#include <applicationcore.h>
#include <components.h>
#include <appfontsettings.h>
#include <qmlapplication.h>
#include <commandlineparser.h>

#include <QApplication>
#include <QtWebView>

QmlApplication* ApplicationCore::s_executionManager = nullptr;

ApplicationCore::ApplicationCore(QObject* parent) : QObject(parent)
{
    // Initialize application
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("interpreter");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");
    QApplication::setApplicationVersion("1.0.0");
    // QApplication::setWindowIcon(QIcon(":/resources/images/owicon.png"));

    /* Set Font */
    AppFontSettings::apply();

    // Init Components
    Components::init();

    // Initialize Web View
    QtWebView::initialize();

    s_executionManager = new QmlApplication(this);
    connect(s_executionManager, &QmlApplication::error,
            this, &ApplicationCore::onError, Qt::QueuedConnection);
    connect(s_executionManager, &QmlApplication::quit,
            qApp, &QApplication::quit, Qt::QueuedConnection);
    connect(s_executionManager, QOverload<int>::of(&QmlApplication::exit),
            this, [=] (int c) { qApp->exit(c); }, Qt::QueuedConnection);
    s_executionManager->exec(CommandlineParser::projectDirectory());
}

void ApplicationCore::onError(const QString& errorString) const
{
    qWarning().noquote() << errorString.trimmed();
    QApplication::exit(EXIT_FAILURE);
}

void ApplicationCore::init(QObject* parent)
{
    static ApplicationCore* instance = nullptr;
    if (instance)
        return;

    instance = new ApplicationCore(parent);
}
