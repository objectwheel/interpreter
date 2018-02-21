#ifndef EXECUTER_H
#define EXECUTER_H

#include <QObject>

class ExecutiveWidget;

class Executer : public QObject
{
        Q_OBJECT
        Q_DISABLE_COPY(Executer)

    public:
        enum Skin {
            Invalid,
            PhonePortrait,
            PhoneLandscape,
            Desktop,
            NoSkin
        };
        Q_ENUM(Skin)

        static void init(const QString& skin);
        static void exec();

    private:
        Executer() {}

    private:
        static Skin _skin;
        static ExecutiveWidget* _eW;
};

#endif // EXECUTER_H
