INCLUDEPATH += $$PWD

HEADERS += $$PWD/aispeak.h \
           $$PWD/apiai.h \
           $$PWD/audioplayer.h \
           $$PWD/audiorecorder.h \
           $$PWD/components.h \
           $$PWD/fm.h

SOURCES += $$PWD/aispeak.cpp \
           $$PWD/apiai.cpp \
           $$PWD/audioplayer.cpp \
           $$PWD/audiorecorder.cpp \
           $$PWD/components.cpp \
           $$PWD/fm.cpp

DEFINES += OW_APIAI \
           OW_AUDIORECORDER \
           OW_AUDIOPLAYER \
           OW_AISPEAK \
           OW_FM