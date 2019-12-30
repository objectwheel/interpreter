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
QT += widgets network qml qml-private quick

INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

SOURCES += $$PWD/main.cpp \
           $$PWD/executer.cpp \
           $$PWD/projectmanager.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/saveutils.cpp \
           $$PWD/quicktheme.cpp

HEADERS += $$PWD/executer.h \
           $$PWD/projectmanager.h \
           $$PWD/qmlcomponent.h \
           $$PWD/qmlcomponent_p.h \
           $$PWD/saveutils.h \
           $$PWD/quicktheme.h

