### App Settings
CONFIG  -= qtquickcompiler
CONFIG  += c++14 strict_c++
TARGET   = Interpreter
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QT += widgets network qml quick webview websockets datavisualization
QT += sensors svg scxml purchasing positioning nfc location gamepad
QT += multimedia webchannel bluetooth quickcontrols2 qml-private
QT += quicktemplates2-private charts
!android:!ios: QT += webenginecore webengine

### Sources
INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

SOURCES += $$PWD/main.cpp \
           $$PWD/qmlapplication.cpp \
           $$PWD/applicationcore.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/saveutils.cpp \
           $$PWD/hashfactory.cpp \
           $$PWD/quicktheme.cpp \
           $$PWD/applicationwindow.cpp \
           $$PWD/broadcastingmanager.cpp \
           $$PWD/view.cpp \
           $$PWD/centralwidget.cpp \
           $$PWD/radarwidget.cpp \
           $$PWD/connectionstatuswidget.cpp \
           $$PWD/connectivitywidget.cpp \
           $$PWD/utilityfunctions.cpp \
           $$PWD/projectmanager.cpp \
           $$PWD/signalwatcher.cpp \
           $$PWD/progressbar.cpp

HEADERS += $$PWD/appconstants.h \
           $$PWD/qmlapplication.h \
           $$PWD/applicationcore.h \
           $$PWD/qmlcomponent.h \
           $$PWD/saveutils.h \
           $$PWD/hashfactory.h \
           $$PWD/quicktheme.h \
           $$PWD/applicationwindow.h \
           $$PWD/broadcastingmanager.h \
           $$PWD/view.h \
           $$PWD/centralwidget.h \
           $$PWD/radarwidget.h \
           $$PWD/connectionstatuswidget.h \
           $$PWD/connectivitywidget.h \
           $$PWD/utilityfunctions.h \
           $$PWD/projectmanager.h \
           $$PWD/signalwatcher.h \
           $$PWD/progressbar.h

ios|android {
    SOURCES += $$PWD/quitbutton.cpp
    HEADERS += $$PWD/quitbutton.h
}

### Includes
include($$PWD/../app.pri)
include($$PWD/interpreter.pri)
include($$PWD/platform/platform.pri)
include($$PWD/resources/resources.pri)
include($$PWD/zipasync/zipasync.pri)
