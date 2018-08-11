### Includes
include($$PWD/platform/platform.pri)
include($$PWD/resources/resources.pri)

### App Settings
TEMPLATE = app
CONFIG  += console
CONFIG  -= app_bundle
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
           $$PWD/commandlineparser.cpp

HEADERS += $$PWD/qmlapplication.h \
           $$PWD/qmlcomponent.h \
           $$PWD/bootsettings.h \
           $$PWD/applicationcore.h \
           $$PWD/commandlineparser.h

### Utils Settings
INCLUDEPATH += $$PWD/../utils
INCLUDEPATH += $$PWD/../utils/fit
INCLUDEPATH += $$PWD/../utils/miniz
INCLUDEPATH += $$PWD/../utils/components
LIBS        += -L$$OUT_PWD/../utils -lutils
windows:CONFIG(release, debug | release):LIBS += -L$$OUT_PWD/../utils/release
windows:CONFIG(debug, debug | release):LIBS += -L$$OUT_PWD/../utils/debug
