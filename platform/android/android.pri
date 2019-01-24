android {
    contains(ANDROID_TARGET_ARCH, x86|armeabi-v7a|arm64-v8a) {
        QT += androidextras
        ANDROID_PACKAGE_SOURCE_DIR = $$PWD
        DISTFILES += $$PWD/AndroidManifest.xml \
                     $$PWD/res/values/libs.xml \
                     $$PWD/src/com/objectwheel/testemulator/TestEmulator.java \

        LIB_SUBDIR = arm
        contains(ANDROID_TARGET_ARCH, x86):LIB_SUBDIR = x86
        ANDROID_EXTRA_LIBS = \
            $$PWD/libs/$$LIB_SUBDIR/libssl.so \
            $$PWD/libs/$$LIB_SUBDIR/libcrypto.so
    } else {
        error("We don't support this arch yet")
    }
}
