VERSION          = 1.0
QT              += core gui widgets network qml quick quickwidgets webview websockets multimedia sensors svg concurrent
TEMPLATE         = app
CONFIG          += c++11 console
CONFIG          -= app_bundle
DEFINES         += QT_DEPRECATED_WARNINGS
TARGET           = objectwheel-interpreter
PROJECT_ROOT     = $$clean_path($$PWD/..)

include($$PWD/detect_build.pri) # BUILD_POSTFIX

!ios {
    BUILD_DIR        = $$PROJECT_ROOT/$$BUILD_POSTFIX
    DESTDIR          = $$BUILD_DIR
    OBJECTS_DIR      = $$BUILD_DIR/.obj
    MOC_DIR          = $$BUILD_DIR/.moc
    RCC_DIR          = $$BUILD_DIR/.rcc
    UI_DIR           = $$BUILD_DIR/.ui
    QMAKE_DISTCLEAN += -r $$PROJECT_ROOT/build
}

SOURCES += main.cpp \
           filemanager.cpp \
           savemanager.cpp

HEADERS += filemanager.h \
           savemanager.h

RESOURCES += resource.qrc

include($$PWD/components/components.pri)

