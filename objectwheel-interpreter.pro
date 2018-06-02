### Includes
include($$PWD/platform/platform.pri)
include($$PWD/resources/resources.pri)

### App Settings
TEMPLATE = app
CONFIG  += console
CONFIG  -= app_bundle
TARGET   = objectwheel-interpreter

QT += widgets network qml qml-private quick webview websockets gamepad webchannel
QT += sensors svg scxml purchasing positioning nfc location bluetooth datavisualization webengine
QT += charts 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation 3dquick multimedia webenginecore

### Sources
INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

SOURCES += $$PWD/main.cpp \
           $$PWD/executer.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/projectmanager.cpp

HEADERS += $$PWD/executer.h \
           $$PWD/qmlcomponent.h \
           $$PWD/qmlcomponent_p.h \
           $$PWD/projectmanager.h

### Utils Settings
INCLUDEPATH += $$PWD/../utils
INCLUDEPATH += $$PWD/../utils/fit
INCLUDEPATH += $$PWD/../utils/miniz
INCLUDEPATH += $$PWD/../utils/components
LIBS        += -L$$OUT_PWD/../utils -lutils
windows:CONFIG(release, debug | release):LIBS += -L$$OUT_PWD/../utils/release
windows:CONFIG(debug, debug | release):LIBS += -L$$OUT_PWD/../utils/debug
