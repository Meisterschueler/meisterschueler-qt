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
    bubblegraphicsitem.cpp \
    bubbleview.cpp \
    guidoview.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    bubblegraphicsitem.h \
    bubbleview.h \
    guidoview.h

# Get Classes
INCLUDEPATH += ../src
VPATH += ../src

SOURCES += midiwrapper.cpp

HEADERS += midiwrapper.h \
           events.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    guidoview.ui

TRANSLATIONS += translations/meisterschueler_de.ts
