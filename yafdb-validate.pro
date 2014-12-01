#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T14:24:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yafdb-validate
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    panoramaviewer.cpp \
    detectedobject.cpp \
    batchview.cpp \
    flowlayout.cpp \
    objectitem.cpp \
    objectrect.cpp \
    util.cpp

HEADERS  += mainwindow.h \
    panoramaviewer.h \
    detectedobject.h \
    batchview.h \
    flowlayout.h \
    objectitem.h \
    objectrect.h \
    util.h \
    types.h

FORMS    += mainwindow.ui \
    batchview.ui \
    objectitem.ui \
    objectrect.ui

LIBS += $$PWD/libs/libgnomonic/lib/libinter/bin/libinter.a \
    $$PWD/libs/libgnomonic/bin/libgnomonic.a \
    -fopenmp

INCLUDEPATH += $$PWD/libs/libgnomonic/lib/libinter/src/ \
    $$PWD/libs/libgnomonic/src/

RESOURCES += \
    resources.qrc


