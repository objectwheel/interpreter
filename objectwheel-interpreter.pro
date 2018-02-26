TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
TARGET = objectwheel-interpreter

QT += widgets network qml qml-private quick quickcontrols2 webview websockets gamepad webchannel
QT += sensors svg scxml purchasing positioning nfc location bluetooth datavisualization webengine
QT += charts 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation 3dquick multimedia webenginecore

SOURCES += $$PWD/main.cpp \
           $$PWD/zipper.cpp \
           $$PWD/executer.cpp \
           $$PWD/saveutils.cpp \
           $$PWD/projectmanager.cpp \
           $$PWD/filemanager.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/executivewidget.cpp

HEADERS += $$PWD/zipper.h \
           $$PWD/executer.h \
           $$PWD/saveutils.h \
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