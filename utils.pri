UTILS_DIR = $$PWD/utils

#INCLUDEPATH += \
#    $$PWD
    
HEADERS += \
    $$UTILS_DIR/alias.h \
    $$UTILS_DIR/breakpad.h \
    $$UTILS_DIR/fileutil.h \
    $$UTILS_DIR/ini.h \
    $$UTILS_DIR/logging.h \
    $$UTILS_DIR/strutil.h \
    $$UTILS_DIR/timeutil.h \
    $$UTILS_DIR/sigslot.h \
    $$UTILS_DIR/stdfs.h \
    $$UTILS_DIR/system.h \
    $$UTILS_DIR/cxxconfig.h \
    $$UTILS_DIR/charset.h \
    $$UTILS_DIR/datetime.h \
    $$UTILS_DIR/process.h


SOURCES += \
    $$UTILS_DIR/alias.cc \
    $$UTILS_DIR/breakpad.cpp \
    $$UTILS_DIR/fileutil.cpp \
    $$UTILS_DIR/logging.cpp \
    $$UTILS_DIR/timeutil.cpp \
    $$UTILS_DIR/stdfs.cpp \
    $$UTILS_DIR/system.cpp \
    $$UTILS_DIR/crashdump.cpp \
    $$UTILS_DIR/charset.cpp \
    $$UTILS_DIR/datetime.cpp \
    $$UTILS_DIR/process.cpp \
    $$UTILS_DIR/process_win.cpp \
    $$UTILS_DIR/ini.cpp \
    $$UTILS_DIR/sigslot.cpp \
    $$UTILS_DIR/strutil.cpp

include(gflags.pri)
include(glog.pri)
