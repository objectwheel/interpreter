TEMPLATE = lib
CONFIG  += plugin c++14 strict_c++
TARGET   = interpreter
DESTDIR  = ../Modules/Objectwheel
QT += widgets network qml qml-private quick
DEFINES += QT_QML_DEBUG_NO_WARNING
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

SOURCES += $$PWD/main.cpp \
           $$PWD/executer.cpp \
           $$PWD/projectmanager.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/saveutils.cpp

HEADERS += $$PWD/executer.h \
           $$PWD/projectmanager.h \
           $$PWD/qmlcomponent.h \
           $$PWD/qmlcomponent_p.h \
           $$PWD/saveutils.h

android {
    DISTFILES += $$PWD/android/AndroidManifest.xml \
                 $$PWD/android/res/values/libs.xml
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

include($$PWD/app.pri)
