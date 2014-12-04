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
    ymlparser.cpp \
    g2g_point.cpp

HEADERS  += mainwindow.h \
    panoramaviewer.h \
    detectedobject.h \
    batchview.h \
    flowlayout.h \
    objectitem.h \
    objectrect.h \
    types.h \
    ymlparser.h \
    g2g_point.h

FORMS    += mainwindow.ui \
    batchview.ui \
    objectitem.ui \
    objectrect.ui

LIBS += $$PWD/libs/libgnomonic/lib/libinter/bin/libinter.a \
    $$PWD/libs/libgnomonic/bin/libgnomonic.a \
    -fopenmp \
    -lopencv_calib3d \
    -lopencv_contrib \
    -lopencv_core \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_gpu \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_legacy \
    -lopencv_ml \
    -lopencv_objdetect \
    -lopencv_ocl \
    -lopencv_photo \
    -lopencv_stitching \
    -lopencv_superres \
    -lopencv_ts \
    -lopencv_video \
    -lopencv_videostab

INCLUDEPATH += $$PWD/libs/libgnomonic/lib/libinter/src/ \
    $$PWD/libs/libgnomonic/src/

RESOURCES += \
    resources.qrc


