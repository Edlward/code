#-------------------------------------------------
#
# Project created by QtCreator 2017-08-02T16:48:38
#
#-------------------------------------------------

QT       += core gui sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtsqlface
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2 \
                /home/lxg/codetest/SeetaFaceEngine/FaceDetection/include \
                /home/lxg/codetest/SeetaFaceEngine/FaceAlignment/include \
                /home/lxg/codetest/SeetaFaceEngine/FaceIdentification/include

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so

#LIBS += -L ./model/ -lseeta_facedet_lib
#LIBS += -L ./model/ -lseeta_fa_lib
#LIBS += -L ./model/ -lviplnet

LIBS += -L/home/lxg/codedata/face/model/ -lseeta_facedet_lib
LIBS += -L/home/lxg/codedata/face/model/ -lseeta_fa_lib
LIBS += -L/home/lxg/codedata/face/model/ -lviplnet
LIBS += -lsqlite3


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    database.cpp \
    face.cpp

HEADERS += \
        mainwindow.h \
    database.h \
    face.h

FORMS += \
        mainwindow.ui
