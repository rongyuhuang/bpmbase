CTPSDK_DIR=$$PWD/ctpsdk

INCLUDEPATH += \
        $$CTPSDK_DIR/include
	
LIBS += $$CTPSDK_DIR/lib/x86/thostmduserapi.lib
LIBS += $$CTPSDK_DIR/lib/x86/thosttraderapi.lib
