CONFIG(debug, debug|release) {
    # gflags
    LIBS += $$VCPKG_LIB/gflags_static.lib
    LIBS += shlwapi.lib
}else{
    # gflags
    LIBS += $$VCPKG_LIB/gflags_static.lib
    LIBS += shlwapi.lib
}
