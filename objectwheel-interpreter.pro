VERSION          = 2.0
TEMPLATE         = app
CONFIG          += c++11
QT              += core gui network qml quick webview websockets multimedia sensors svg concurrent
TARGET           = objectwheel-interpreter

SOURCES += main.cpp \
           fit.cpp \
           miniz.cpp \
           zipper.cpp \
           executer.cpp \
    projectmanager.cpp

HEADERS += fit.h \
           miniz.h \
           zipper.h \
           executer.h \
    projectmanager.h

include($$PWD/components/components.pri)

