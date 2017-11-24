TEMPLATE = app #lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += static_runtime

SOURCES += client.cc

include(../msvc.pri)
include(../gflags.pri)
include(../glog.pri)
include(../libevent.pri)
include(../evpp.pri)

# copy files
#QMAKE_POST_LINK += $$copyToBuilddir($$VCPKG_BIN/*.dll)
