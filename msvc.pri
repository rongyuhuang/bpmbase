# warn
win32-msvc*:QMAKE_CXXFLAGS_WARN_ON -= -w34100
win32-msvc*:QMAKE_CXXFLAGS += -wd4819 -wd4267 -wd4244 -wd4091 -wd4018 -wd4828 -wd4996
win32-msvc*:QMAKE_CFLAGS_WARN_ON -= -w34100
win32-msvc*:QMAKE_CFLAGS += -wd4819 -wd4267 -wd4244 -wd4091 -wd4018 -wd4828 -wd4996

# NOMINMAX WIN32_LEAN_AND_MEAN
DEFINES += NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE

# UNICODE
DEFINES += UNICODE
DEFINES += _UNICODE

# defines win7+
DEFINES += _WIN32_WINNT=0x0600

# fuck the same name for cpps
CONFIG += no_batch

# pdb
CONFIG += force_debug_info

# static runtime
#QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO -= -MD
#QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= -MD
#QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO += -MT
#QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO += -MT
#CONFIG += static_runtime

# postthreadmessage
win32:LIBS += user32.lib

# c++11
CONFIG += c++11

# x64 | x86
contains(QT_ARCH, i386) {
    #message("32-bit")
    DEFINES += WIN32
    DEFINES += _WIN32
    DEFINES += __WIN32__
    #error("dont support x86")
} else {
    #message("64-bit")
    DEFINES += WIN64
    DEFINES += _WIN64
    DEFINES += __WIN64__
}

# encoding
# There is also a /utf-8 option that is a synonym for setting
#  “/source-charset:utf-8” and “/execution-charset:utf-8”.
QMAKE_CFLAGS += -utf-8
QMAKE_CXXFLAGS += -utf-8

# parallel complie
QMAKE_CXXFLAGS += -MP
QMAKE_CXXFLAGS += -MP

# src目录下相对路径包含=
INCLUDEPATH += \
    $$PWD

# vcpkg目录=
VCPKG_DIR = D:/projects/vcpkg/installed/x86-windows-static
INCLUDEPATH += \
    $$VCPKG_DIR/include

CONFIG(debug, debug|release) {
    #TARGET = debug_binary
    #message("debug")
    VCPKG_LIB = $$VCPKG_DIR/debug/lib
    VCPKG_BIN = $$VCPKG_DIR/debug/bin
} else {
    #message("release")
    #TARGET = release_binary
    VCPKG_LIB = $$VCPKG_DIR/lib
    VCPKG_BIN = $$VCPKG_DIR/bin
}

# 拷贝文件=
defineReplace(copyToDir) {
    files = $$1
    DIR = $$2
    LINK =

    for(FILE, files) {
        LINK += $$QMAKE_COPY $$shell_path($$FILE) $$shell_path($$DIR) $$escape_expand(\\n\\t)
    }
    return($$LINK)
}

defineReplace(copyToBuilddir) {
    CONFIG(debug, debug|release) {
        return($$copyToDir($$1, $$OUT_PWD/debug))
    }else{
        return($$copyToDir($$1, $$OUT_PWD/release))
    }
}
