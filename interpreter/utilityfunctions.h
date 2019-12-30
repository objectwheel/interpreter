#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

class QFont;
class QObject;

namespace UtilityFunctions {

QFont systemDefaultFont();

bool isAnyChildWindowFullScreen(QObject* parentObject);

} // UtilityFunctions

#endif // UTILITYFUNCTIONS_H