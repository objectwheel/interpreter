#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QString>

class CommandlineParser final
{
    Q_DISABLE_COPY(CommandlineParser)

public:
    static void init(int argc, char* argv[]);
    static QString projectDirectory();

private:
    CommandlineParser() {}

private:
    static QString s_projectDirectory;
};

#endif // COMMANDLINEPARSER_H