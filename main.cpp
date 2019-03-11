#include <applicationcore.h>
#include <QApplication>

// TODO: Handle multiple Objectwheel IDE connection attempts

int main(int argc, char* argv[])
{
    // Prepare core
    ApplicationCore::prepare();

    // Initialize application
    QApplication app(argc, argv);

    // Run core
    ApplicationCore core;
    // core.startQmlApplication("/users/omergoktas/desktop/cloudy");
    // Start main event loop
    return app.exec();
}