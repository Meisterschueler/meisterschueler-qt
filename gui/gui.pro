#-------------------------------------------------
#
# Project created by QtCreator 2013-05-11T14:35:35
#
#-------------------------------------------------

include(../common.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    ../src/midiwrapper.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    ../src/midiwrapper.h

FORMS    += mainwindow.ui \
    settingsdialog.ui

TRANSLATIONS += translations/meisterschueler_de.ts
