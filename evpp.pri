CONFIG(debug, debug|release) {
    # evpp
    LIBS += $$VCPKG_LIB/evpp_static.lib
}else{
    # evpp
    LIBS += $$VCPKG_LIB/evpp_static.lib
}
