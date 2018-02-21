#ifndef EXECUTER_H
#define EXECUTER_H

#include <QList>
#include <QQmlError>
#include <QString>

enum Skin {
    Invalid,
    PhonePortrait,
    PhoneLandscape,
    Desktop,
    NoSkin
};

namespace Executer
{
    void exec();
}

#endif // EXECUTER_H
