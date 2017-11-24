TEMPLATE = app #lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += static_runtime

SOURCES += main.cpp

include(../msvc.pri)
include(../gtest.pri)

# copy files
#QMAKE_POST_LINK += $$copyToBuilddir($$VCPKG_BIN/*.dll)
