#include <applicationcore.h>
#include <cleanexit.h>
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
    Q_UNUSED(core)

    // Start main event loop
    return CleanExit::exec(app);
}