#ifndef CROSSPLATFORM_H
#define CROSSPLATFORM_H

#include <QString>

namespace CrossPlatform {
bool isAndroidEmulator();
bool isIosEmulator();
bool isEmulator();
QString deviceName();
void restart();
}
#endif // CROSSPLATFORM_H