DEFINES += APP_VER='"\\\"$$VERSION\\\""' \
           APP_NAME='"\\\"$$TARGET\\\""' \
           APP_CORP='"\\\"Objectwheel, Inc.\\\""' \
           APP_DOMAIN='"\\\"objectwheel.com\\\""'
# FIXME

#Copy modules
macx {
    modules.files = $$OUT_PWD/../modules/Modules
    modules.path = Contents/Frameworks
    QMAKE_BUNDLE_DATA += modules
} else:android {
    ANDROID_EXTRA_PLUGINS += $$OUT_PWD/../modules/Modules
}
