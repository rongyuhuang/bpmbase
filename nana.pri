CONFIG(debug, debug|release) {
    # nana
    LIBS += $$VCPKG_LIB/nana_d.lib
    LIBS += $$VCPKG_LIB/libpng16d.lib
    LIBS += $$VCPKG_LIB/jpeg.lib
    LIBS += $$VCPKG_LIB/zlibd.lib
    LIBS += user32.lib gdi32.lib shell32.lib comdlg32.lib
}else{
    # nana
    LIBS += $$VCPKG_LIB/nana.lib
    LIBS += $$VCPKG_LIB/libpng16.lib
    LIBS += $$VCPKG_LIB/jpeg.lib
    LIBS += $$VCPKG_LIB/zlib.lib
    LIBS += user32.lib gdi32.lib shell32.lib comdlg32.lib
}
