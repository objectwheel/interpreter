include($$PWD/ios/ios.pri)
include($$PWD/macx/macx.pri)
include($$PWD/android/android.pri)

DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

HEADERS += $$PWD/crossplatform.h
SOURCES += $$PWD/crossplatform.cpp
