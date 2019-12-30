### Includes
include($$PWD/interpreter.pri)
include($$PWD/platform/platform.pri)

### App Settings
TEMPLATE = app
CONFIG  -= qtquickcompiler
CONFIG  += c++14 strict_c++
TARGET   = Interpreter
DEFINES += QT_QML_DEBUG_NO_WARNING
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
QT += widgets network qml qml-private quick quickcontrols2

INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

SOURCES += $$PWD/main.cpp \
           $$PWD/executer.cpp \
           $$PWD/applicationcore.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/saveutils.cpp \
           $$PWD/quicktheme.cpp \
           $$PWD/utilityfunctions.cpp

HEADERS += $$PWD/executer.h \
           $$PWD/applicationcore.h \
           $$PWD/qmlcomponent.h \
           $$PWD/saveutils.h \
           $$PWD/quicktheme.h \
           $$PWD/utilityfunctions.h
