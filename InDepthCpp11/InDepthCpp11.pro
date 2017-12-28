TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp\
    ch1.cpp \
    ch4.cpp \
    ch5.cpp \
    ch6.cpp \
    ch9.cpp \
    ch16.cpp

include(../msvc.pri)
include(../myboost.pri)
HEADERS += \
    iteratorimp.h \
    syncqueue.h \
    server.h \
    message.h \
    rwhandler.h \
    connector.h
