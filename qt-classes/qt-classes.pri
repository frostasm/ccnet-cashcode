CONFIG += qt
CONFIG -= console

QT += core gui widgets

FORMS += \
    $$PWD/MainWindow.ui

HEADERS += \
    $$PWD/MainWindow.h \
    $$PWD/CashCodeBillValidator.h \
    $$PWD/CashCodeBillValidatorBase.h

SOURCES += \
    $$PWD/MainWindow.cpp \
    $$PWD/CashCodeBillValidator.cpp \
    $$PWD/qt-main.cpp \
    $$PWD/CashCodeBillValidatorBase.cpp
