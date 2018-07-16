TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cashcodeprotocol.cpp \
    serialport.cpp \
    ccpackage.cpp

HEADERS += cashcodeprotocol.h \
    cashcodeerrors.h \
    serialport.h \
    ccpackage.h \
    commands.h \
    timingspecifications.h

windows: {
    LIBS += -lws2_32
#    LIBS += -LH:\Libs\bin \
    LIBS += \
            -lboost_system-mgw53-mt-d-x32-1_67 \
            -lboost_thread-mgw53-mt-d-x32-1_67 \
            -lboost_chrono-mgw53-mt-d-x32-1_67
} else {
    LIBS += \
            -lboost_system \
            -lboost_thread \
            -lboost_chrono
}
