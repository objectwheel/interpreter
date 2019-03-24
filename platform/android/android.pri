QT += androidextras

DISTFILES += $$PWD/AndroidManifest.xml \
             $$PWD/res/values/libs.xml \
             $$PWD/src/com/objectwheel/deviceinfo/DeviceInfo.java \

ANDROID_PACKAGE_SOURCE_DIR = $$PWD

ANDROID_EXTRA_LIBS = \
    $$PWD/libs/$$ANDROID_TARGET_ARCH/libssl.so \
    $$PWD/libs/$$ANDROID_TARGET_ARCH/libcrypto.so