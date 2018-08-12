#include <applicationcore.h>
#include <bootsettings.h>
#include <commandlineparser.h>

#include <QApplication>
#include <QSharedPointer>

int main(int argc, char* argv[])
{
    // Parse commandline arguments
    CommandlineParser::init(argc, argv);

    // Apply boot settings
    BootSettings::apply();

    // Initialize application
    QSharedPointer<QGuiApplication> app(BootSettings::useGuiApplication()
            ? new QGuiApplication(argc, argv)
            : new QApplication(argc, argv));

    // Initialize application core
    ApplicationCore::init(app.data());

    return app->exec();
}