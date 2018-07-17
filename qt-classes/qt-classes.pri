CONFIG += qt
CONFIG -= console

QT += core gui widgets

FORMS += \
    $$PWD/MainWindow.ui

HEADERS += \
    $$PWD/MainWindow.h \
    $$PWD/CashCodeBillValidatorBase.h \
    $$PWD/CashCodeBillValidatorSingleton.h \
    $$PWD/CashCodeBillValidatorThreaded.h \
    $$PWD/CashCodeBillValidatorWorker.h \
    $$PWD/CashCodeBillValidatorTester.h

SOURCES += \
    $$PWD/MainWindow.cpp \
    $$PWD/qt-main.cpp \
    $$PWD/CashCodeBillValidatorBase.cpp \
    $$PWD/CashCodeBillValidatorSingleton.cpp \
    $$PWD/CashCodeBillValidatorThreaded.cpp \
    $$PWD/CashCodeBillValidatorWorker.cpp \
    $$PWD/CashCodeBillValidatorTester.cpp
