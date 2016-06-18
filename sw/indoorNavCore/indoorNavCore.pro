QT += core
QT -= gui

CONFIG += c++11 static

TARGET = indoorNavCore
CONFIG += console
CONFIG -= app_bundle


LIBS += /home/uli/x-tools/arm-cortex_a8-linux-gnueabi/arm-cortex_a8-linux-gnueabi/sysroot/lib/libGeographic.a #FOR ARM BUILD

#LIBS += /usr/local/lib/libGeographic.a  #FOR HOST BUILD

TEMPLATE = app

SOURCES += \
    initialconfig.cpp \
    main.cpp \
    global.cpp \
    CL/clistener.cpp \
    FPL/cfiltercontrol.cpp \
    FPL/cfilter.cpp \
    CL/clistenerthread.cpp \
    FPL/cmedianfilter.cpp \
    capplicationthread.cpp \
    FPL/cmeanfilter.cpp \
    APL/cleastsquaretriangulator.cpp \
    MATH_TOOLS/mathtools.cpp \
    clog.cpp \
    HAL/cprotocoldriver.cpp \
    HAL/cnmeaprotocoldriver.cpp \
    APL/ctriangulator.cpp \
    APL/clevenbergmarquarttriangulator.cpp

HEADERS += \
    initialconfig.h \
    global.h \
    CL/clistener.h \
    FPL/cfiltercontrol.h \
    FPL/cfilter.h \
    CL/clistenerthread.h \
    FPL/cmedianfilter.h \
    capplicationthread.h \
    FPL/cmeanfilter.h \
    APL/cleastsquaretriangulator.h \
    MATH_TOOLS/mathtools.h \
    MATH_TOOLS/constants.h \
    clog.h \
    HAL/cprotocoldriver.h \
    HAL/cnmeaprotocoldriver.h \
    APL/ctriangulator.h \
    APL/clevenbergmarquarttriangulator.h

