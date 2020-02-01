QT += androidextras

DISTFILES += $$PWD/src/com/objectwheel/deviceinfo/DeviceInfo.java \
             $$PWD/AndroidManifest.xml \
             $$PWD/res/values/libs.xml \
             $$PWD/res/drawable-ldpi/icon.png \
             $$PWD/res/drawable-mdpi/icon.png \
             $$PWD/res/drawable-hdpi/icon.png \
             $$PWD/res/drawable-xhdpi/icon.png \
             $$PWD/res/drawable-xxhdpi/icon.png \
             $$PWD/res/drawable-xxxhdpi/icon.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD

ANDROID_EXTRA_LIBS = \
    $$PWD/lib/$$ANDROID_TARGET_ARCH/libssl_1_1.so \
    $$PWD/lib/$$ANDROID_TARGET_ARCH/libcrypto_1_1.so