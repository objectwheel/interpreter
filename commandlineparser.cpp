#include <commandlineparser.h>
#include <filemanager.h>

QString CommandlineParser::s_projectDirectory;

void CommandlineParser::init(int argc, char* argv[])
{
    Q_UNUSED(argc);
#if defined(Q_OS_ANDROID)
    s_projectDirectory = "assets:";
#else
    s_projectDirectory = dname(argv[0]);
#endif
}

QString CommandlineParser::projectDirectory()
{
    return s_projectDirectory;
}