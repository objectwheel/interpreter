#include <deviceinfo.h>
#include <UIKit/UIDevice.h>

namespace Ios {
namespace DeviceInfo {

QString deviceName()
{    
    const QString& name = QString::fromNSString([[UIDevice currentDevice] name]);
    if (name.startsWith("iphone", Qt::CaseInsensitive)
            || name.startsWith("ipad", Qt::CaseInsensitive)
            || name.startsWith("ipod", Qt::CaseInsensitive)) {
        return "Apple " + name;
    } else {
        return name;
    }
}

}
}
