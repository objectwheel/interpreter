QT += gui-private ### For QuitButton
DEPENDPATH      += $$PWD
INCLUDEPATH     += $$PWD
QMAKE_INFO_PLIST = $$PWD/Info.plist

HEADERS         += $$PWD/deviceinfo.h \
                   $$PWD/windowoperations.h

SOURCES         += $$PWD/deviceinfo.mm \
                   $$PWD/windowoperations.mm
