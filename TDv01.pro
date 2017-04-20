#-------------------------------------------------
#
# Project created by QtCreator 2014-02-28T14:42:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TDv01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    map.cpp \
    control.cpp \
    minion.cpp \
    mapscene.cpp \
    building.cpp \
    bullet.cpp

HEADERS  += mainwindow.h \
    map.h \
    control.h \
    minion.h \
    mapscene.h \
    building.h \
    bullet.h \
    Defines.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -O2
