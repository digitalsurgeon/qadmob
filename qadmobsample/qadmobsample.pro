#-------------------------------------------------
#
# Project created by QtCreator 2010-05-02T21:21:32
#
#-------------------------------------------------

TARGET = qadmobsample
TEMPLATE = app
QT += network
DESTDIR = ../debug

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += ../inc

win32:
{
    LIBS += ../debug/libqadmob.a
}
