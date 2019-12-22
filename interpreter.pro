CONFIG      += ordered
TEMPLATE     = subdirs

OTHER_FILES += $$PWD/TODO

SUBDIRS      = $$PWD/modules/modules.pro \
               $$PWD/interpreter/interpreter.pro
