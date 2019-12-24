QT += androidextras

DISTFILES += $$PWD/src/com/objectwheel/deviceinfo/DeviceInfo.java \
             $$PWD/AndroidManifest.xml \
             $$PWD/res/values/libs.xml \
             $$PWD/res/mipmap-ldpi/ic_launcher.png \
             $$PWD/res/mipmap-mdpi/ic_launcher.png \
             $$PWD/res/mipmap-hdpi/ic_launcher.png \
             $$PWD/res/mipmap-xhdpi/ic_launcher.png \
             $$PWD/res/mipmap-xxhdpi/ic_launcher.png \
             $$PWD/res/mipmap-xxxhdpi/ic_launcher.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD

ANDROID_EXTRA_LIBS = \
    $$PWD/libs/$$ANDROID_TARGET_ARCH/libssl.so \
    $$PWD/libs/$$ANDROID_TARGET_ARCH/libcrypto.so