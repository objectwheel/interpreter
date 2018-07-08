#ifndef EXECUTER_H
#define EXECUTER_H

#include <QObject>

class Executer : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Executer)

public:
    static Executer* instance();
    void exec();

private:
    Executer() {}
};

#endif // EXECUTER_H
