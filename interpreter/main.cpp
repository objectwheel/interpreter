#include <applicationcore.h>
#include <QApplication>

int main(int argc, char* argv[])
{
    // Prepare core
    ApplicationCore::prepare();

    // Initialize application
    QApplication app(argc, argv);

    if (ApplicationCore::locked())
        return EXIT_FAILURE;

    // Run core
    ApplicationCore core;
    core.run();

    // Start main event loop
    return app.exec();
}