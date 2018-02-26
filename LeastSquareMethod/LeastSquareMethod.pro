#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T07:58:15
#
#-------------------------------------------------

QT       += core gui
CONFIG+= console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LeastSquareMethod
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    matrix.cpp \
    inputtable.cpp \
    data.cpp \
    figure.cpp

HEADERS  += mainwindow.h \
    matrix.h \
    inputtable.h \
    data.h \
    figure.h \
    def.h

FORMS    += mainwindow.ui
