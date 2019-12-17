### Includes
include($$PWD/platform/platform.pri)
include($$PWD/resources/resources.pri)

### App Settings
TEMPLATE = app
CONFIG  -= app_bundle
CONFIG  -= qtquickcompiler
CONFIG  += console c++14 strict_c++
TARGET   = Interpreter
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QT += widgets network qml quick webview websockets datavisualization
QT += sensors svg scxml purchasing positioning nfc location gamepad
QT += charts multimedia webenginecore webengine webchannel bluetooth
QT += qml-private quicktemplates2-private

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
include($$PWD/../objectwheel.pri)
INCLUDEPATH += $$PWD/../utils
LIBS        += -L$$OUT_PWD/../utils -lutils
windows:CONFIG(release, debug | release):LIBS += -L$$OUT_PWD/../utils/release
windows:CONFIG(debug, debug | release):LIBS += -L$$OUT_PWD/../utils/debug
