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

enum ExecErrorType {
    NoError,
    CommonError,
    ChildIsWindowError,
    MasterIsNonGui,
    FormIsNonGui,
    MainFormIsntWindowError,
    MultipleWindowsForMobileError,
    NoMainForm,
    CodeError
};

struct ExecError {
        ExecErrorType type = NoError;
        QList<QQmlError> errors;
        QString id;
};

namespace Executer
{
    ExecError execProject();
}

#endif // EXECUTER_H
