#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

#include <utility>
#include <QDataStream>

namespace UtilityFunctions {

namespace Internal {

void pushHelper(QDataStream&);
void pullHelper(QDataStream&);

template <typename Arg, typename... Args>
void pushHelper(QDataStream& stream, Arg&& arg, Args&&... args)
{
    stream << std::forward<Arg>(arg);
    pushHelper(stream, std::forward<Args>(args)...);
}

template <typename Arg, typename... Args>
void pullHelper(QDataStream& stream, Arg&& arg, Args&&... args) {
    stream >> std::forward<Arg>(arg);
    pullHelper(stream, std::forward<Args>(args)...);
}

} // Internal

template <typename... Args>
QByteArray push(Args&&... args) {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_12);
    Internal::pushHelper(stream, std::forward<Args>(args)...);
    return data;
}

template <typename... Args>
void pull(const QByteArray& data, Args&&... args) {
    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_5_12);
    Internal::pullHelper(stream, std::forward<Args>(args)...);
}

} // UtilityFunctions

#endif // UTILITYFUNCTIONS_H