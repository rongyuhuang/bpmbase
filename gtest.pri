CONFIG(debug, debug|release) {
    # gtest
    LIBS += $$VCPKG_LIB/manual-link/gtest.lib
    LIBS += $$VCPKG_LIB/manual-link/gtest_main.lib
}else{
    # gtest
    LIBS += $$VCPKG_LIB/manual-link/gtest.lib
    LIBS += $$VCPKG_LIB/manual-link/gtest_main.lib
}
