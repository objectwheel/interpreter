### Includes
include($$PWD/resources/resources.pri)
include($$PWD/components/components.pri)

### App Settings
TEMPLATE = app
TARGET   = interpreter
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QT += widgets network qml qml-private quick webview websockets gamepad webchannel
QT += sensors svg scxml purchasing positioning nfc location bluetooth datavisualization webengine
QT += charts 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation 3dquick multimedia webenginecore

### Sources
INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

SOURCES += $$PWD/main.cpp \
           $$PWD/qmlapplication.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/applicationcore.cpp \
           $$PWD/bootsettings.cpp \
           $$PWD/commandlineparser.cpp \
           $$PWD/filemanager.cpp \
           $$PWD/saveutils.cpp \
           $$PWD/hashfactory.cpp \
           $$PWD/appfontsettings.cpp \
           $$PWD/quicktheme.cpp

HEADERS += $$PWD/qmlapplication.h \
           $$PWD/qmlcomponent.h \
           $$PWD/bootsettings.h \
           $$PWD/applicationcore.h \
           $$PWD/commandlineparser.h \
           $$PWD/filemanager.h \
           $$PWD/saveutils.h \
           $$PWD/hashfactory.h \
           $$PWD/appfontsettings.h \
           $$PWD/quicktheme.h

android {
    DISTFILES += $$PWD/android/AndroidManifest.xml \
                 $$PWD/android/res/values/libs.xml
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

ios|macx {
    ios {
        project.path =
    } else {
        project.path = Contents/MacOS
    }
    project.files = /Users/omergoktas/Desktop/owtestsuite/owdb /Users/omergoktas/Desktop/owtestsuite/project.json
    QMAKE_BUNDLE_DATA += project
}