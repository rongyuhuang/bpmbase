MYBOOST_DIR = $$PWD/myboost

INCLUDEPATH += \
    $$MYBOOST_DIR
    
DEFINES += \
    BOOST_ALL_NO_LIB

# 带*的路径，windows下必须用\，而不是/
SOURCES += \
  #$$MYBOOST_DIR\libs\date_time\src\gregorian\*.cpp \
  $$MYBOOST_DIR\libs\filesystem\src\*.cpp \
  $$MYBOOST_DIR\libs\system\src\*.cpp

# Use Precompiled headers (PCH)
# PRECOMPILED_HEADER = $$PWD/myboost_precompile.h
