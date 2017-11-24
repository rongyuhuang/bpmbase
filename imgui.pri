CONFIG(debug, debug|release) {
    # imgui
    LIBS += $$VCPKG_LIB/imguid.lib
}else{
    # imgui
    LIBS += $$VCPKG_LIB/imgui.lib
}

IMGUI_DIR = $$PWD/imgui
HEADERS += \
    $$IMGUI_DIR/guimain.h

include($$IMGUI_DIR/imgui_dx9/imgui_dx9.pri)
