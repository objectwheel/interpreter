### App Settings
TEMPLATE = app
TARGET   = interpreter
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QT += widgets network qml qml-private quick webview websockets gamepad webchannel
QT += sensors svg scxml purchasing positioning nfc location bluetooth datavisualization
QT += charts 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation 3dquick multimedia

### Sources
INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

SOURCES += $$PWD/main.cpp \
           $$PWD/qmlapplication.cpp \
           $$PWD/applicationcore.cpp \
           $$PWD/commandlineparser.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/saveutils.cpp \
           $$PWD/hashfactory.cpp \
           $$PWD/quicktheme.cpp \
           $$PWD/applicationwindow.cpp \
           $$PWD/discoverymanager.cpp \
           $$PWD/view.cpp \
           $$PWD/centralwidget.cpp \
           $$PWD/radarwidget.cpp \
    connectionstatuswidget.cpp \
    connectivitywidget.cpp

HEADERS += $$PWD/qmlapplication.h \
           $$PWD/applicationcore.h \
           $$PWD/commandlineparser.h \
           $$PWD/qmlcomponent.h \
           $$PWD/saveutils.h \
           $$PWD/hashfactory.h \
           $$PWD/quicktheme.h \
           $$PWD/applicationwindow.h \
           $$PWD/discoverymanager.h \
           $$PWD/view.h \
           $$PWD/centralwidget.h \
           $$PWD/radarwidget.h \
    connectionstatuswidget.h \
    connectivitywidget.h

### Includes
include($$PWD/resources/resources.pri)
include($$PWD/components/components.pri)
include($$PWD/platform/platform.pri)
