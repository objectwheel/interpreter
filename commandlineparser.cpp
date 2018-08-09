#include <commandlineparser.h>

QString CommandlineParser::s_projectDirectory;

void CommandlineParser::init(int argc, char* argv[])
{
    Q_ASSERT_X(argc == 2, "init", "Number of total commandline arguments must be equal to 2");
    s_projectDirectory = argv[1];
}

QString CommandlineParser::projectDirectory()
{
    return s_projectDirectory;
}