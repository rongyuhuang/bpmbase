CONFIG(debug, debug|release) {
    # fmt
    LIBS += $$VCPKG_LIB/fmt_static.lib
}else{
    # fmt
    LIBS += $$VCPKG_LIB/fmt_static.lib
}
