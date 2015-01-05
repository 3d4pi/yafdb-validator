#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T14:24:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yafdb-validate
TEMPLATE = app

RESOURCES += \
    resources.qrc

INCLUDEPATH += $$PWD/include/ \
    $$PWD/libs/libgnomonic/lib/libinter/src/ \
    $$PWD/libs/libgnomonic/src/

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/panoramaviewer.cpp \
    src/detectedobject.cpp \
    src/batchview.cpp \
    src/flowlayout.cpp \
    src/objectitem.cpp \
    src/ymlparser.cpp \
    src/g2g_point.cpp \
    src/objectrect.cpp \
    src/editview.cpp \
    src/etg_point.cpp

HEADERS  += include/mainwindow.h \
    include/panoramaviewer.h \
    include/detectedobject.h \
    include/batchview.h \
    include/flowlayout.h \
    include/objectitem.h \
    include/types.h \
    include/ymlparser.h \
    include/g2g_point.h \
    include/objectrect.h \
    include/editview.h \
    include/etg_point.h

FORMS    += ui/mainwindow.ui \
    ui/batchview.ui \
    ui/objectitem.ui \
    ui/editview.ui

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


