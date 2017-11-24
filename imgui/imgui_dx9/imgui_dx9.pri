HEADERS += \
    $$PWD/imgui_impl_dx9.h

SOURCES += \
    $$PWD/guimain_dx9.cpp \
    $$PWD/imgui_impl_dx9.cpp

LIBS += d3d9.lib

#DEFINES += IMGUI_DISABLE_TEST_WINDOWS
