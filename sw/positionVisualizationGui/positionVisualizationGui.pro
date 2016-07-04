#-------------------------------------------------
#
# Project created by QtCreator 2016-06-20T17:26:41
#
#-------------------------------------------------

QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = positionVisualizationGui
TEMPLATE = app


SOURCES += main.cpp\
        cpositionvisualizationgui.cpp \
    qcustomplot.cpp

HEADERS  += cpositionvisualizationgui.h \
    qcustomplot.h

FORMS    += cpositionvisualizationgui.ui
