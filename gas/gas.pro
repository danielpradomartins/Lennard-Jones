#-------------------------------------------------
#
# Project created by QtCreator 2015-06-14T16:39:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gas
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ambiente.cpp \
    particula.cpp \
    arquivos.cpp

HEADERS  += mainwindow.h \
    ambiente.h \
    particula.h \
    arquivos.h

FORMS    += mainwindow.ui
