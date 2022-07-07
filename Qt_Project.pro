#-------------------------------------------------
#
# Project created by QtCreator 2021-01-23T12:22:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_Project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    avltree.cpp \
    avlwindow.cpp \
    bstwindow.cpp \
    cbtwindow.cpp \
        main.cpp \
    item.cpp \
    mainwindow.cpp \
    rectproxywidget.cpp \
    renderarea.cpp \
    sortwindow.cpp \
    titlebar.cpp

HEADERS += \
    avltree.h \
    avlwindow.h \
    binarysearchtree.h \
    bstwindow.h \
    cbtwindow.h \
    item.h \
    mainwindow.h \
    rectproxywidget.h \
    renderarea.h \
    sortwindow.h \
    titlebar.h

FORMS += \
    avlwindow.ui \
    cbtwindow.ui \
    item.ui \
    mainwindow.ui \
    sortwindow.ui \
    titlebar.ui

RESOURCES += \
    res.qrc
