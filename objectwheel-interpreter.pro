TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
TARGET = objectwheel-interpreter

QT += widgets network qml qml-private quick quickcontrols2 webview websockets webengine webchannel
QT += sensors svg scxml purchasing positioning nfc multimedia location gamepad datavisualization
QT += charts 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation 3dquick bluetooth webenginecore

SOURCES += $$PWD/main.cpp \
           $$PWD/zipper.cpp \
           $$PWD/executer.cpp \
           $$PWD/projectmanager.cpp \
           $$PWD/filemanager.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/executivewidget.cpp

HEADERS += $$PWD/zipper.h \
           $$PWD/executer.h \
           $$PWD/projectmanager.h \
           $$PWD/filemanager.h \
           $$PWD/qmlcomponent.h \
           $$PWD/qmlcomponent_p.h \
           $$PWD/executivewidget.h

include($$PWD/fit/fit.pri)
include($$PWD/miniz/miniz.pri)
include($$PWD/components/components.pri)
include($$PWD/platform/platform.pri)
include($$PWD/resources/resources.pri)