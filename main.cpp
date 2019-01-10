#include <applicationcore.h>
#include <commandlineparser.h>
#include <QApplication>

int main(int argc, char* argv[])
{
    // Parse commandline arguments
    CommandlineParser::init(argc, argv);

    // Prepare core
    ApplicationCore::prepare();

    // Initialize application
    QScopedPointer<QGuiApplication> app(ApplicationCore::useGuiApplication()
            ? new QGuiApplication(argc, argv)
            : new QApplication(argc, argv));

    // Run core
    ApplicationCore core;
    core.run();

    // Start main event loop
    return app->exec();
}