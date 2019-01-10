#include <commandlineparser.h>
#include <filemanager.h>

QString CommandlineParser::s_projectDirectory;

QString CommandlineParser::projectDirectory()
{
    return s_projectDirectory;
}

void CommandlineParser::init(int /*argc*/, char* argv[])
{
#if defined(Q_OS_ANDROID)
    s_projectDirectory = "assets:";
#else
    s_projectDirectory = dname(argv[0]);
#endif
}
