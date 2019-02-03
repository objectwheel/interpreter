ios:include($$PWD/ios/ios.pri)
android:include($$PWD/android/android.pri)

DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

HEADERS += $$PWD/crossplatform.h
SOURCES += $$PWD/crossplatform.cpp

# TODO: Add platform icons and launch images