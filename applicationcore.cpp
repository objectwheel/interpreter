#include <applicationcore.h>
#include <components.h>
#include <appfontsettings.h>
#include <executionmanager.h>

#include <QApplication>
#include <QtWebView>

ExecutionManager* ApplicationCore::s_executionManager = nullptr;

ApplicationCore::ApplicationCore(QObject* parent) : QObject(parent)
{
    // Initialize application
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("interpreter");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setWindowIcon(QIcon(":/resources/images/owicon.png"));

    s_executionManager = new ExecutionManager(this);

    /* Set Font */
    AppFontSettings::apply();

    // Init Components
    Components::init();

    // Initialize Web View
    QtWebView::initialize();

    ExecutionManager::exec();
}

void ApplicationCore::init(QObject* parent)
{
    static ApplicationCore* instance = nullptr;
    if (instance)
        return;

    instance = new ApplicationCore(parent);
}
