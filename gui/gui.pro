#-------------------------------------------------
#
# Project created by QtCreator 2013-05-11T14:35:35
#
#-------------------------------------------------

include(../common.pri)

QT       += core gui

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

SOURCES += echomanager.cpp \
           guidoservice.cpp \
           hanonsongfactory.cpp \
           matchinghandler.cpp \
           matchingitem.cpp \
           matchingservice.cpp \
           merginghandler.cpp \
           midiservice.cpp \
           midiwrapper.cpp \
           needlemanwunsch.cpp \
           notemapper.cpp \
           playbackhandler.cpp \
           resultmanager.cpp \
           score.cpp \
           scoreservice.cpp \
           signalmanager.cpp \
           song.cpp \
           songservice.cpp \
           events.cpp

HEADERS += echomanager.h \
           guidoservice.h \
           hanonsongfactory.h \
           matchinghandler.h \
           matchingitem.h \
           matchingservice.h \
           merginghandler.h \
           midiservice.h \
           midiwrapper.h \
           needlemanwunsch.h \
           notemapper.h \
           playbackhandler.h \
           resultmanager.h \
           score.h \
           scoreservice.h \
           signalmanager.h \
           song.h \
           songservice.h \
           events.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    guidoview.ui

TRANSLATIONS += translations/meisterschueler_de.ts
