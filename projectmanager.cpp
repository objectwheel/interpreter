#include <projectmanager.h>
#include <filemanager.h>

namespace ProjectManager {
QString g_argv, g_baseDir;

QString projectDirectory()
{
    if (g_baseDir.isEmpty()) {
#if defined(Q_OS_ANDROID)
        g_baseDir = "assets:";
#else
        g_baseDir = dname(g_argv);
#endif
    }
        return g_baseDir;
}

void init(const QString& argv)
{
    g_argv = argv;
}
} // ProjectManager