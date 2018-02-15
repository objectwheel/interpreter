TEMPLATE = app
QT += network qml quick quickcontrols2

SOURCES += $$PWD/main.cpp \
           $$PWD/zipper.cpp \
           $$PWD/executer.cpp \
           $$PWD/projectmanager.cpp \
           $$PWD/filemanager.cpp

HEADERS += $$PWD/zipper.h \
           $$PWD/executer.h \
           $$PWD/projectmanager.h \
           $$PWD/filemanager.h

android {
    DISTFILES += $$PWD/android/AndroidManifest.xml \
                 $$PWD/android/res/values/libs.xml
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

#include($$PWD/app.pri)
include($$PWD/fit/fit.pri)
include($$PWD/miniz/miniz.pri)
#include($$PWD/components/components.pri)