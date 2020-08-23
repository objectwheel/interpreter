### App Settings
CONFIG  += cmdline c++14 strict_c strict_c++ utf8_source hide_symbols
TARGET   = Interpreter
gcc:QMAKE_CXXFLAGS += -pedantic-errors
msvc:QMAKE_CXXFLAGS += -permissive-
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

QT += widgets network qml quick webview websockets datavisualization
QT += sensors svg scxml purchasing positioning nfc location gamepad
QT += charts multimedia webenginecore webengine webchannel bluetooth
QT += qml-private quicktemplates2-private

### Sources
INCLUDEPATH += $$PWD

SOURCES += $$PWD/main.cpp \
           $$PWD/qmlapplication.cpp \
           $$PWD/applicationcore.cpp \
           $$PWD/commandlineparser.cpp \
           $$PWD/qmlcomponent.cpp

HEADERS += $$PWD/qmlapplication.h \
           $$PWD/applicationcore.h \
           $$PWD/commandlineparser.h \
           $$PWD/qmlcomponent.h

RESOURCES += $$PWD/resources/resources.qrc

### Includes
include($$PWD/../app.pri)
include($$PWD/platform/platform.pri)

### Utils Settings
INCLUDEPATH += $$PWD/../utils
LIBS        += -L$$OUT_PWD/../utils -lUtils
windows:CONFIG(release, debug | release):LIBS += -L$$OUT_PWD/../utils/release
windows:CONFIG(debug, debug | release):LIBS += -L$$OUT_PWD/../utils/debug
