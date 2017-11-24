TEMPLATE = app #lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += static_runtime

SOURCES += main.cpp guimgr.cpp

include(../msvc.pri)
include(../utils.pri)
include(../imgui.pri)
include(../libevent.pri)

# copy files
#QMAKE_POST_LINK += $$copyToBuilddir($$VCPKG_BIN/*.dll)
