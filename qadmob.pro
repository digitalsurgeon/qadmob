# -------------------------------------------------
# Project created by QtCreator 2010-04-24T09:32:20
# -------------------------------------------------

QT += network
QT -= gui
TARGET = qadmob
TEMPLATE = lib
DESTDIR = ./debug
DEFINES += QADMOB_LIBRARY
VERSION = 0.1


INCLUDEPATH += inc/

SOURCES += src/qadmob.cpp \
    src/qadmobwidget.cpp

HEADERS += inc/qadmob.h \
    inc/qadmobwidget.h

# Please set the path to qjson here, for all platforms you want to build it for
INCLUDEPATH += ../qjson/src
win32: {
    LIBS += ../qjson/lib/libqjson0.a
}

symbian: {
    INCLUDEPATH += /epoc32/include/stdapis
    TARGET.EPOCALLOWDLLDATA = 1 # Symbian specific
    TARGET.CAPABILITY = NetworkServices # Symbian specific, we just need network access

    LIBS += -lqjson

    lqjson.sources = qjson.dll
    lqjson.path  = \sys\bin
    DEPLOYMENT += lqjson
}
