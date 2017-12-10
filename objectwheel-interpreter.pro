TEMPLATE = app
QT += network qml quick quickcontrols2

SOURCES += main.cpp \
           zipper.cpp \
           executer.cpp \
           projectmanager.cpp \
           filemanager.cpp

HEADERS += zipper.h \
           executer.h \
           projectmanager.h \
           filemanager.h

android {
    DISTFILES += android/AndroidManifest.xml \
                 android/res/values/libs.xml
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

include($$PWD/app.pri)
include($$PWD/fit/fit.pri)
include($$PWD/miniz/miniz.pri)
include($$PWD/components/components.pri)
