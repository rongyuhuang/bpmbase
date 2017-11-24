CONFIG(debug, debug|release) {
    # glog
    LIBS += $$VCPKG_LIB/glog.lib
}else{
    # glog
    LIBS += $$VCPKG_LIB/glog.lib
}
