QT += core
QT -= gui

CONFIG += c++11

LIBS   += /home/uli/x-tools/arm-cortex_a8-linux-gnueabi/arm-cortex_a8-linux-gnueabi/sysroot/lib/libiw.a

TARGET = wifiDriver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS +=
