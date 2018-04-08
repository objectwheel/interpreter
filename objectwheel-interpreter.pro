### Includes
include($$PWD/platform/platform.pri)
include($$PWD/resources/resources.pri)
include($$PWD/../shared/resources/shared_resources.pri)

### App Settings
TEMPLATE = app
CONFIG  += console
CONFIG  -= app_bundle
TARGET   = objectwheel-interpreter

QT += widgets network qml qml-private quick quickcontrols2 webview websockets gamepad webchannel
QT += sensors svg scxml purchasing positioning nfc location bluetooth datavisualization webengine
QT += charts 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation 3dquick multimedia webenginecore

### Sources
INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

SOURCES += $$PWD/main.cpp \
           $$PWD/executer.cpp \
           $$PWD/qmlcomponent.cpp \
           $$PWD/projectmanager.cpp \
           $$PWD/executivewidget.cpp

HEADERS += $$PWD/executer.h \
           $$PWD/qmlcomponent.h \
           $$PWD/qmlcomponent_p.h \
           $$PWD/projectmanager.h \
           $$PWD/executivewidget.h

### Utils Settings
INCLUDEPATH += $$PWD/../shared
INCLUDEPATH += $$PWD/../utils
INCLUDEPATH += $$PWD/../utils/fit
INCLUDEPATH += $$PWD/../utils/miniz
INCLUDEPATH += $$PWD/../utils/components
LIBS        += -L$$OUT_PWD/../utils -lutils
windows:CONFIG(release, debug | release):LIBS += -L$$OUT_PWD/../utils/release
windows:CONFIG(debug, debug | release):LIBS += -L$$OUT_PWD/../utils/debug
