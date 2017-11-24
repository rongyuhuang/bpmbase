DEFINES += ZMQ_STATIC

CONFIG(debug, debug|release) {
    # zeromq
    LIBS += $$VCPKG_LIB/libzmq-mt-sgd-4_2_3.lib
    LIBS += advapi32.lib ws2_32.lib iphlpapi.lib
}else{
    # zeromq
    LIBS += $$VCPKG_LIB/libzmq-mt-s-4_2_3.lib
    LIBS += advapi32.lib ws2_32.lib iphlpapi.lib
}
