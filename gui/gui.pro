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
    guidoview.cpp \
    chordview.cpp \
    feedbackdialog.cpp \
    graphicsscoreitem.cpp \
    customview.cpp \
    timelineview.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    bubblegraphicsitem.h \
    bubbleview.h \
    guidoview.h \
    chordwidget.h \
    feedbackdialog.h \
    graphicsscoreitem.h \
    customview.h \
    timelineview.h

# Get Classes
INCLUDEPATH += ../src
VPATH += ../src

SOURCES += clusterhandler.cpp \
           commandmanager.cpp \
           feedbackmanager.cpp \
           guidoservice.cpp \
           hanonsongfactory.cpp \
           matchinghandler.cpp \
           matchingitem.cpp \
           matchingservice.cpp \
           merginghandler.cpp \
           midiservice.cpp \
           midiwrapper.cpp \
           mymapcollector.cpp \
           needlemanwunsch.cpp \
           notemapper.cpp \
           playbackhandler.cpp \
           recordhandler.cpp \
           resultmanager.cpp \
           score.cpp \
           scoreservice.cpp \
           signalmanager.cpp \
           song.cpp \
           songservice.cpp \
           statisticsservice.cpp \
           events.cpp

HEADERS += clusterhandler.h \
           commandmanager.h \
           feedbackmanager.h \
           guidoservice.h \
           hanonsongfactory.h \
           matchinghandler.h \
           matchingitem.h \
           matchingservice.h \
           merginghandler.h \
           midiservice.h \
           midiwrapper.h \
           mymapcollector.h \
           needlemanwunsch.h \
           notemapper.h \
           playbackhandler.h \
           recordhandler.h \
           resultmanager.h \
           score.h \
           scoreservice.h \
           signalmanager.h \
           song.h \
           songservice.h \
           statisticsservice.h \
           events.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    guidoview.ui \
    feedbackdialog.ui

TRANSLATIONS += translations/meisterschueler_de.ts \
    translations/meisterschueler_fr.ts

RESOURCES += \
    resources.qrc
