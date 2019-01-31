#include <crossplatform.h>
#include <QObject>

#if defined(Q_OS_ANDROID)
#include <QAndroidJniObject>
#endif

#if defined(Q_OS_IOS)
#include <deviceinfo.h>
#endif

namespace CrossPlatform {

bool isAndroidEmulator()
{
#if defined(Q_OS_ANDROID)
    static const bool emulator = QAndroidJniObject::callStaticMethod<jboolean>(
                "com/objectwheel/deviceinfo/DeviceInfo", "isEmulator");
#else
    static const bool emulator = false;
#endif
    return emulator;
}

bool isIosEmulator()
{
#if defined(Q_OS_IOS) && defined(__APPLE_EMBEDDED_SIMULATOR__)
    return true;
#else
    return false;
#endif
}

bool isEmulator()
{
#if defined(Q_OS_ANDROID)
    return isAndroidEmulator();
#elif defined(Q_OS_IOS)
    return isIosEmulator();
#else
    return false;
#endif
}

QString deviceName()
{
#if defined(Q_OS_ANDROID)
    static const QString deviceName = QAndroidJniObject(QAndroidJniObject::callStaticObjectMethod<jstring>(
                                                            "com/objectwheel/deviceinfo/DeviceInfo",
                                                            "getDeviceName")).toString();
#elif defined(Q_OS_IOS)
    static const QString deviceName = Ios::DeviceInfo::deviceName();
#else
    static const QString deviceName = QObject::tr("Unknown");
#endif
    return deviceName;
}
}
