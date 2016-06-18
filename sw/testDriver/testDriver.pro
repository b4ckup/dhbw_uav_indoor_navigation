QT += core
QT -= gui

CONFIG += c++11

TARGET = testDriver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ctestdriver.cpp

HEADERS += \
    ctestdriver.h
