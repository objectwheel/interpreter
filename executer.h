#ifndef EXECUTER_H
#define EXECUTER_H

#include <saveutils.h>

class ExecutiveWidget;

class Executer : public QObject
{
        Q_OBJECT
        Q_DISABLE_COPY(Executer)

    public:
        static Executer* instance();
        void init(const QString& skin);
        void exec();

    private:
        Executer();

    private:
        SaveUtils::Skin _skin;
        ExecutiveWidget* _eW;
};

#endif // EXECUTER_H
