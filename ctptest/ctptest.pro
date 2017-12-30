TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include(../ctpsdk.pri)
include(../easyctp.pri)
include(../msvc.pri)
