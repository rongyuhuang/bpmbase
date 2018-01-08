KBARUTILS_DIR = $$PWD/kbarutils

#INCLUDEPATH += \
#    $$PWD

HEADERS += \
    $$KBARUTILS_DIR/kbartable.h \
    $$KBARUTILS_DIR/mempiece.h \
    $$KBARUTILS_DIR/tickstorage.h \
    $$KBARUTILS_DIR/kbarstorage.h \
    $$KBARUTILS_DIR/kbarconvert.h \
    $$KBARUTILS_DIR/arraymath.h \
    $$KBARUTILS_DIR/smbase.h \
    $$KBARUTILS_DIR/lowess.h

SOURCES += \
    $$KBARUTILS_DIR/kbartable.cpp \
    $$KBARUTILS_DIR/tickstorage.cpp \
    $$KBARUTILS_DIR/kbarstorage.cpp \
    $$KBARUTILS_DIR/kbarconvert.cpp \
    $$KBARUTILS_DIR/arraymath.cpp

#include(easyctpsdk.pri)
#include(myboost.pri)
#include(utils.pri)
