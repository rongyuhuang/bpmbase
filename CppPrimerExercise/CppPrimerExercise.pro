TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cppprimerch1.cpp \
    cppprimerch2.cpp \
    cppprimerch4.cpp \
    cppprimerch5.cpp \
    cppprimerch6.cpp \
    cppprimerch7.cpp \
    sales_data.cpp \
    cppprimerch8.cpp \
    cppprimerch9.cpp \
    cppprimerch10.cpp \
    cppprimerch11.cpp \
    cppprimerch12.cpp \
    strblob.cpp

HEADERS += \
    Sales_item.h \
    sales_data.h \
    screen.h \
    strblob.h

include(../msvc.pri)
