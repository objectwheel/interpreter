#include <crossplatform.h>
#include <QApplication>

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
#include <QProcess>
#endif

#if defined(Q_OS_ANDROID)
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QDateTime>
#endif

#if defined(Q_OS_IOS)
#include <deviceinfo.h>
#endif

#if defined(Q_OS_MACOS)
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
#elif defined(Q_OS_MACOS)
    static const QString deviceName = Macos::DeviceInfo::deviceName();
#else
    static const QString deviceName = QObject::tr("Unknown");
#endif
    return deviceName;
}

void restart()
{
#if defined(Q_OS_IOS)
    qWarning("WARNING: CrossPlatform::restart() only quits on IOS");
#elif defined(Q_OS_ANDROID)
    auto activity = QtAndroid::androidActivity();
    auto packageManager = activity.callObjectMethod(
                "getPackageManager",
                "()Landroid/content/pm/PackageManager;");
    auto activityIntent = packageManager.callObjectMethod(
                "getLaunchIntentForPackage",
                "(Ljava/lang/String;)Landroid/content/Intent;",
                activity.callObjectMethod("getPackageName", "()Ljava/lang/String;").object());
    auto pendingIntent = QAndroidJniObject::callStaticObjectMethod(
                "android/app/PendingIntent", "getActivity",
                "(Landroid/content/Context;ILandroid/content/Intent;I)Landroid/app/PendingIntent;",
                activity.object(), jint(0), activityIntent.object(),
                QAndroidJniObject::getStaticField<jint>("android/content/Intent", "FLAG_ACTIVITY_CLEAR_TOP"));
    auto alarmManager = activity.callObjectMethod(
                "getSystemService",
                "(Ljava/lang/String;)Ljava/lang/Object;",
                QAndroidJniObject::getStaticObjectField("android/content/Context",
                                                        "ALARM_SERVICE",
                                                        "Ljava/lang/String;").object());
    alarmManager.callMethod<void>(
                "set", "(IJLandroid/app/PendingIntent;)V",
                QAndroidJniObject::getStaticField<jint>("android/app/AlarmManager", "RTC"),
                jlong(QDateTime::currentMSecsSinceEpoch() + 100), pendingIntent.object());
#else
    QProcess::startDetached(qApp->applicationFilePath());
#endif
    QMetaObject::invokeMethod(qApp, &QApplication::quit, Qt::QueuedConnection);
    qApp->quit();
}

}
