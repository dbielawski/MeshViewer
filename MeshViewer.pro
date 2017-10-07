#-------------------------------------------------
#
# Project created by QtCreator 2017-09-09T14:54:39
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MINOR_VERSION, 8)

TARGET = MeshViewer
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

INCLUDEPATH += include


SOURCES +=  source/main.cpp\
    source/mainwindow.cpp \
    source/octree.cpp \
    source/mesh.cpp \
    source/pgm3d.cpp \
    source/glwidget.cpp \
    source/camera.cpp \
    source/scene.cpp \
    source/light.cpp \
    source/obj.cpp \
    source/alignedbox3f.cpp \
    source/wireboundingbox.cpp

HEADERS  += include/mainwindow.h \
    include/utils.h \
    include/octree.h \
    include/mesh.h \
    include/pgm3d.h \
    include/glwidget.h \
    include/camera.h \
    include/scene.h \
    include/model3d.h \
    include/light.h \
    include/obj.h \
    include/alignedbox3f.h \
    include/wireboundingbox.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

