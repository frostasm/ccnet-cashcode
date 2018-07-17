TEMPLATE = app
CONFIG += c++11

SOURCES += \
    cashcodeprotocol.cpp \
    serialport.cpp \
    ccpackage.cpp

HEADERS += cashcodeprotocol.h \
    cashcodeerrors.h \
    serialport.h \
    ccpackage.h \
    commands.h \
    timingspecifications.h \
    IBillValidatorEventsListener.h

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

MAIN_CPP=main.cpp
QT_CLASSES_PRI=

contains(DEFINES, WITH_QT): {
    message(Building with Qt)
    DEFINES += QT_DEPRECATED_WARNINGS
    QT_CLASSES_PRI=qt-classes/qt-classes.pri
    MAIN_CPP=
} else {
    message(Building without Qt)
    CONFIG -= qt
    CONFIG += console
    CONFIG -= app_bundle
    QT_CLASSES_PRI=
}

include($${QT_CLASSES_PRI})

SOURCES += $${MAIN_CPP}
