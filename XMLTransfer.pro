#-------------------------------------------------
#
# Project created by QtCreator 2016-06-23T15:18:46
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XMLTransfer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    xmlhandle.cpp \
    filehandle.cpp \
    utunit.cpp \
    wasuhandle.cpp

HEADERS  += mainwindow.h \
    xmlhandle.h \
    filehandle.h \
    utunit.h \
    wasuhandle.h

FORMS    += mainwindow.ui
