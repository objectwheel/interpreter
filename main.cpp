#include <executer.h>
#include <components.h>
#include <filemanager.h>
#include <projectmanager.h>
#include <saveutils.h>
#include <fit.h>

#include <QtWidgets>

#if defined(QT_WEBVIEW_LIB)
#include <QtWebView>
#endif

#define PIXEL_SIZE 14
#define REF_DPI 149.0

namespace {
    QString setTheme(const QString& projectDir);
}

int main(int argc, char* argv[])
{
    // Initialize fit library
    fit::update(REF_DPI);

    if (argc > 1)
        setTheme(argv[1]);

    // Boot settings
    qputenv("QML_DISABLE_DISK_CACHE", "true");

    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL); // For web view tool tips
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Initialize application
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Objectwheel");
    QApplication::setOrganizationDomain("objectwheel.com");
    QApplication::setApplicationName("objectwheel-interpreter");
    QApplication::setApplicationDisplayName("Objectwheel Interpreter");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setWindowIcon(QIcon(":/resources/images/owicon.png"));

    QFont font;
    font.setPixelSize(fit::fx(PIXEL_SIZE));
    font.setStyleStrategy(QFont::PreferAntialias);
    #if defined(Q_OS_MACOS)
      font.setFamily(".SF NS Display");
    #elif defined(Q_OS_WIN)
      font.setFamily("Segoe UI");
    #else
      font.setFamily("Open Sans");
    #endif
    QApplication::setFont(font);

    // Init Components
    Components::init();

    // Initialize Web View
    #if defined(QT_WEBVIEW_LIB)
    QtWebView::initialize();
    #endif

    if (argc > 1)
        ProjectManager::instance()->init(argv[1]);

    // Start
    Executer::instance()->exec();

    return a.exec();
}


namespace {
    QString setTheme(const QString& projectDir)
    {
        const auto& object = SaveUtils::theme(projectDir).toObject();
        const auto& version = object.value("version").toString();
        const auto& style = object.value("style").toString();

        if (version == "v1") {
            qputenv("QT_QUICK_CONTROLS_1_STYLE", style.toUtf8().constData());
        } else {
            const auto& theme = object.value("theme").toString();
            const auto& accent = object.value("accent").toString();
            const auto& primary = object.value("primary").toString();
            const auto& foreground = object.value("foreground").toString();
            const auto& background = object.value("background").toString();

            qputenv("QT_QUICK_CONTROLS_STYLE", style.toUtf8().constData());

            if (style == QString("Material")) {
                if (!theme.isEmpty())
                    qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", theme.toUtf8().constData());

                if (!accent.isEmpty())
                    qputenv("QT_QUICK_CONTROLS_MATERIAL_ACCENT", accent.toUtf8().constData());

                if (!primary.isEmpty())
                    qputenv("QT_QUICK_CONTROLS_MATERIAL_PRIMARY", primary.toUtf8().constData());

                if (!foreground.isEmpty())
                    qputenv("QT_QUICK_CONTROLS_MATERIAL_FOREGROUND", foreground.toUtf8().constData());

                if (!background.isEmpty())
                    qputenv("QT_QUICK_CONTROLS_MATERIAL_BACKGROUND", background.toUtf8().constData());
            } else if (style == QString("Universal")) {
                if (!theme.isEmpty())
                    qputenv("QT_QUICK_CONTROLS_UNIVERSAL_THEME", theme.toUtf8().constData());

                if (!accent.isEmpty())
                    qputenv("QT_QUICK_CONTROLS_UNIVERSAL_ACCENT", accent.toUtf8().constData());

                if (!foreground.isEmpty())
                    qputenv("QT_QUICK_CONTROLS_UNIVERSAL_FOREGROUND", foreground.toUtf8().constData());

                if (!background.isEmpty())
                    qputenv("QT_QUICK_CONTROLS_UNIVERSAL_BACKGROUND", background.toUtf8().constData());
            }
        }

        return version;
    }
}
