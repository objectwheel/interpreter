#Copy modules

macx {
    modules.files = $$OUT_PWD/../modules/Modules
    modules.path = Contents/Frameworks
    QMAKE_BUNDLE_DATA += modules
} else:android {
    ANDROID_EXTRA_PLUGINS += $$OUT_PWD/../modules/Modules
}
