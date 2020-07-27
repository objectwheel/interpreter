#Copy modules
macx {
    modules.files = $$OUT_PWD/../modules/Modules
    modules.path = Contents/Frameworks
    QMAKE_BUNDLE_DATA += modules
} else:android {
    ANDROID_EXTRA_PLUGINS += $$OUT_PWD/../modules/Modules
}

#Copy project
exists($$PWD/Project/Project.qrc) {
    RESOURCES += $$PWD/Project/Project.qrc
} else:exists($$PWD/Project.rcc) {
    macx {
        project.files = $$PWD/Project.rcc
        project.path = Contents/Resources
        QMAKE_BUNDLE_DATA += project
    } else:android {
        project.files = $$PWD/Project.rcc
        project.path = /assets
        INSTALLS += project
    }
} else {
    warning("Unable to find project file!!!")
}
