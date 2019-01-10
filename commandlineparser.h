#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QString>

class CommandlineParser final
{
public:
    static QString projectDirectory();
    static void init(int argc, char* argv[]);

private:
    static QString s_projectDirectory;

public:
    CommandlineParser() = delete;
    CommandlineParser(const CommandlineParser&) = delete;
    CommandlineParser& operator=(const CommandlineParser&) = delete;
};

#endif // COMMANDLINEPARSER_H