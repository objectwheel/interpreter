DEFINES += APP_VER='"\\\"$$VERSION\\\""' \
           APP_NAME='"\\\"$$TARGET\\\""' \
           APP_CORP='"\\\"Objectwheel, Inc.\\\""' \
           APP_DOMAIN='"\\\"objectwheel.com\\\""'

touch(applicationcore.cpp, interpreter.pri)

#Copy modules
macx {
    modules.files = $$OUT_PWD/../modules/Modules
    modules.path = Contents/Frameworks
    QMAKE_BUNDLE_DATA += modules
} else:android {
    ANDROID_EXTRA_PLUGINS += $$OUT_PWD/../modules/Modules
}

#Copy project
exists($$PWD/Project.rcc) {
    macx {
        project.files = $$PWD/Project.rcc
        project.path = Contents/Resources
        QMAKE_BUNDLE_DATA += project
    } else:android {
        ASSETS = $$PWD/platform/android/assets
        mkpath($$ASSETS)
        DESTINATION_DIR = $$shell_quote($$shell_path($$ASSETS))
        FILES_TO_COPY = $$PWD/Project.rcc
        for (FILE, FILES_TO_COPY) {
            FILE_PATH = $$shell_quote($$shell_path($$FILE))
            QMAKE_POST_LINK += $$QMAKE_COPY $$FILE_PATH $$DESTINATION_DIR $$escape_expand(\n\t)
        }
    }
} else {
    warning("Unable to find project file!!!")
}