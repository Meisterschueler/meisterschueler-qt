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

SOURCES += guidoservice.cpp \
           hanonsongfactory.cpp \
           matchinghandler.cpp \
           matchingitem.cpp \
           matchingservice.cpp \
           merginghandler.cpp \
           midiservice.cpp \
           midiwrapper.cpp \
           needlemanwunsch.cpp \
           notemapper.cpp \
           score.cpp \
           scoreservice.cpp \
           song.cpp \
           songservice.cpp \
           events.cpp

HEADERS += guidoservice.h \
           hanonsongfactory.h \
           matchinghandler.h \
           matchingitem.h \
           matchingservice.h \
           merginghandler.h \
           midiservice.h \
           midiwrapper.h \
           needlemanwunsch.h \
           notemapper.h \
           score.h \
           scoreservice.h \
           song.h \
           songservice.h \
           events.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    guidoview.ui

TRANSLATIONS += translations/meisterschueler_de.ts
