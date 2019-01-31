#include <deviceinfo.h>
#include <UIKit/UIDevice.h>

namespace Ios {

namespace DeviceInfo {

QString deviceName()
{
    return "Apple " + QString::fromNSString([[UIDevice currentDevice] name]);
}
}
}
