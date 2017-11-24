CONFIG(debug, debug|release) {
    # libevent
    LIBS += $$VCPKG_LIB/event.lib
    LIBS += $$VCPKG_LIB/event_core.lib
    LIBS += $$VCPKG_LIB/event_extra.lib
    LIBS += advapi32.lib shell32.lib ws2_32.lib
}else{
    # libevent
    LIBS += $$VCPKG_LIB/event.lib
    LIBS += $$VCPKG_LIB/event_core.lib
    LIBS += $$VCPKG_LIB/event_extra.lib
    LIBS += advapi32.lib shell32.lib ws2_32.lib
}
