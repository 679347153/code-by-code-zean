#-------------------------------------------------
#
# Project created by QtCreator 2023-04-22T21:02:59
#
#-------------------------------------------------

QT       += core gui multimedia sql network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = learn-to-use-V1-0
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mybutton.cpp \
    database.cpp

HEADERS  += mainwindow.h \
    mybutton.h \
    database.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
CONFIG += resources_big

DISTFILES += \
    json/object.json \
    database.json
