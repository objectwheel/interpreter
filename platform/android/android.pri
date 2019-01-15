android {
    contains(ANDROID_TARGET_ARCH, x86|armeabi-v7a|arm64-v8a) {
        ANDROID_PACKAGE_SOURCE_DIR = $$PWD
        DISTFILES += $$PWD/AndroidManifest.xml \
                     $$PWD/res/values/libs.xml

        LIB_SUBDIR = arm
        contains(ANDROID_TARGET_ARCH, x86):LIB_SUBDIR = x86
        ANDROID_EXTRA_LIBS = \
            $$PWD/libs/$$LIB_SUBDIR/libssl.so \
            $$PWD/libs/$$LIB_SUBDIR/libcrypto.so

        installProject.files = $$files($$PWD/../../project/*)
        installProject.path = /assets/owprj
        INSTALLS += installProject
    } else {
        error("We don't support this arch yet")
    }
}
