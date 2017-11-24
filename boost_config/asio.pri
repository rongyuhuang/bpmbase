ASIO_DIR = $$PWD/asio

INCLUDEPATH += \
    $$ASIO_DIR/include
    
HEADERS += \
    $$ASIO_DIR/include/asio.hpp
    
DEFINES += ASIO_STANDALONE

