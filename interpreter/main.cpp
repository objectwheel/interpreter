#include <applicationcore.h>
#include <QApplication>

int main(int argc, char* argv[])
{
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
