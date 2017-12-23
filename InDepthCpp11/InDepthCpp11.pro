TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp\
    ch1.cpp \
    ch4.cpp \
    ch5.cpp

include(../msvc.pri)

HEADERS += \
    iteratorimp.h
