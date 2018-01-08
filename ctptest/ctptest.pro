TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += static_runtime

SOURCES += main.cpp \
    datareceiver.cpp

include(../ctpsdk.pri)
include(../easyctp.pri)
include(../msvc.pri)
include(../utils.pri)
include(../fmt.pri)
include(../kbarutils.pri)
HEADERS += \
    datareceiver.h \
    config.h \
    simplesyncqueue.h
