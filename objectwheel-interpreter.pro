TEMPLATE = app
QT += network qml quick

SOURCES += main.cpp \
           zipper.cpp \
           executer.cpp \
           projectmanager.cpp

HEADERS += zipper.h \
           executer.h \
           projectmanager.h

include($$PWD/app.pri)
include($$PWD/fit/fit.pri)
include($$PWD/miniz/miniz.pri)
include($$PWD/components/components.pri)
