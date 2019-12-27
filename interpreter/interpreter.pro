TEMPLATE = app
QT += gui widgets network qml qml-private quick

SOURCES += $$PWD/main.cpp \
           $$PWD/executer.cpp \
           $$PWD/projectmanager.cpp \
           $$PWD/filemanager.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/saveutils.cpp \
           $$PWD/hashfactory.cpp

HEADERS += $$PWD/executer.h \
           $$PWD/projectmanager.h \
           $$PWD/filemanager.h \
           $$PWD/qmlcomponent.h \
           $$PWD/qmlcomponent_p.h \
           $$PWD/saveutils.h \
           $$PWD/hashfactory.h

android {
    DISTFILES += $$PWD/android/AndroidManifest.xml \
                 $$PWD/android/res/values/libs.xml
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

include($$PWD/app.pri)
include($$PWD/miniz/miniz.pri)
include($$PWD/resources/resources.pri)
include($$PWD/components/components.pri)