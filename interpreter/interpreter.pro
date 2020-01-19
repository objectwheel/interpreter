### Includes
include($$PWD/interpreter.pri)
include($$PWD/platform/platform.pri)

### App Settings
CONFIG  -= qtquickcompiler
CONFIG  += c++14 strict_c++
TARGET   = Interpreter
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
QT += svg widgets network qml quick quickcontrols2 qml-private quicktemplates2-private
### QT += qtModules

INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

SOURCES     += $$PWD/main.cpp \
               $$PWD/qmlapplication.cpp \
               $$PWD/applicationcore.cpp \
               $$PWD/qmlcomponent.cpp \
               $$PWD/saveutils.cpp \
               $$PWD/quicktheme.cpp \
               $$PWD/utilityfunctions.cpp

HEADERS     += $$PWD/qmlapplication.h \
               $$PWD/applicationcore.h \
               $$PWD/qmlcomponent.h \
               $$PWD/saveutils.h \
               $$PWD/quicktheme.h \
               $$PWD/utilityfunctions.h

OTHER_FILES += $$PWD/server.json

### NOTE: If we add qrc files, qmlimportscanner and androiddeployqt
### cannot properly detect qml imports based on the user project we
### inject into the current directory (called Project directory)
