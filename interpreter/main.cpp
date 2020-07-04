#include <applicationcore.h>
#include <QApplication>

int main(int argc, char* argv[])
{
    // Set those here, needed by QStandardPaths
    QApplication::setApplicationName(QStringLiteral("Objectwheel Interpreter"));
    QApplication::setOrganizationName(QStringLiteral("Objectwheel"));
    QApplication::setApplicationVersion(QStringLiteral("3.0"));
    QApplication::setOrganizationDomain(QStringLiteral("objectwheel.com"));
    QApplication::setApplicationDisplayName(QStringLiteral("Objectwheel Interpreter"));

    // Prepare core
    ApplicationCore::prepare();

    // Initialize application
    QApplication app(argc, argv);

    // Run core
    ApplicationCore core;
    Q_UNUSED(core)

    // Start main event loop
    return app.exec();
}
