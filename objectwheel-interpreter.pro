VERSION  = 2.0
TEMPLATE = app
TARGET   = objectwheel-interpreter
QT      += network qml quick webview websockets multimedia sensors svg concurrent

SOURCES += main.cpp \
           zipper.cpp \
           executer.cpp \
           projectmanager.cpp

HEADERS += zipper.h \
           executer.h \
           projectmanager.h

include($$PWD/fit/fit.pri)
include($$PWD/miniz/miniz.pri)
include($$PWD/components/components.pri)
