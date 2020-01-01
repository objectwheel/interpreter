QT += androidextras

DISTFILES += $$PWD/AndroidManifest.xml \
             $$PWD/res/values/libs.xml \
             $$PWD/res/values/styles.xml \
             $$PWD/res/drawable/splash.9.png \
             $$PWD/res/drawable-ldpi/splash.9.png \
             $$PWD/res/drawable-mdpi/splash.9.png \
             $$PWD/res/drawable-hdpi/splash.9.png \
             $$PWD/res/drawable-xhdpi/splash.9.png \
             $$PWD/res/drawable-xxhdpi/splash.9.png \
             $$PWD/res/drawable-xxxhdpi/splash.9.png \
             $$PWD/res/drawable/icon.png \
             $$PWD/res/drawable-ldpi/icon.png \
             $$PWD/res/drawable-mdpi/icon.png \
             $$PWD/res/drawable-hdpi/icon.png \
             $$PWD/res/drawable-xhdpi/icon.png \
             $$PWD/res/drawable-xxhdpi/icon.png \
             $$PWD/res/drawable-xxxhdpi/icon.png \
             $$PWD/res/drawable/launch_background.xml \
             $$PWD/res/drawable/normal_background.xml \
             $$PWD/src/com/objectwheel/interpreter/InterpreterActivity.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD

ANDROID_EXTRA_LIBS = \
    $$PWD/libs/$$ANDROID_TARGET_ARCH/libssl_1_1.so \
    $$PWD/libs/$$ANDROID_TARGET_ARCH/libcrypto_1_1.so
