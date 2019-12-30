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
    QT += androidextras
    DISTFILES += $$PWD/android/AndroidManifest.xml \
                 $$PWD/android/res/values/libs.xml \
                 $$PWD/android/res/values/styles.xml \
                 $$PWD/android/res/drawable/launch_background.xml \
                 $$PWD/android/res/drawable/normal_background.xml \
                 $$PWD/android/res/drawable/ic_splash.9.png \
                 $$PWD/android/res/drawable-ldpi/ic_splash.9.png \
                 $$PWD/android/res/drawable-mdpi/ic_splash.9.png \
                 $$PWD/android/res/drawable-hdpi/ic_splash.9.png \
                 $$PWD/android/res/drawable-xhdpi/ic_splash.9.png \
                 $$PWD/android/res/drawable-xxhdpi/ic_splash.9.png \
                 $$PWD/android/res/drawable-xxxhdpi/ic_splash.9.png \
                 $$PWD/android/res/drawable-ldpi/icon.png \
                 $$PWD/android/res/drawable-mdpi/icon.png \
                 $$PWD/android/res/drawable-hdpi/icon.png \
                 $$PWD/android/res/drawable-xhdpi/icon.png \
                 $$PWD/android/res/drawable-xxhdpi/icon.png \
                 $$PWD/android/res/drawable-xxxhdpi/icon.png \
                 $$PWD/android/src/com/objectwheel/interpreter/InterpreterActivity.java

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

include($$PWD/app.pri)
