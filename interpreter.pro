### Includes
include($$PWD/platform/platform.pri)
include($$PWD/resources/resources.pri)

### App Settings
TEMPLATE = app
CONFIG  -= app_bundle
CONFIG  += console c++14 strict_c++
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
           $$PWD/qmlcomponent.cpp

HEADERS += $$PWD/qmlapplication.h \
           $$PWD/applicationcore.h \
           $$PWD/commandlineparser.h \
           $$PWD/qmlcomponent.h

### Utils Settings
INCLUDEPATH += $$PWD/../utils
INCLUDEPATH += $$PWD/../utils/components
RESOURCES   += $$PWD/../utils/resources/shared/moduleresolver/moduleresolver.qrc
LIBS        += -L$$OUT_PWD/../utils -lutils
windows:CONFIG(release, debug | release):LIBS += -L$$OUT_PWD/../utils/release
windows:CONFIG(debug, debug | release):LIBS += -L$$OUT_PWD/../utils/debug
