#-------------------------------------------------
#
# Project created by QtCreator 2013-06-04T12:15:18
#
#-------------------------------------------------

include(../common.pri)

QT       += core gui

TARGET = minimal
TEMPLATE = app

SOURCES += main.cpp

HEADERS  +=

FORMS    +=

# Get Classes
INCLUDEPATH += ../src
VPATH += ../src

SOURCES += events.cpp \
    clusterhandler.cpp \
    graphicsscoreitem.cpp \
    guidoservice.cpp \
    scoreservice.cpp \
    hanonsongfactory.cpp \
    matchinghandler.cpp \
    matchingservice.cpp \
    matchingitem.cpp \
    merginghandler.cpp \
    midiservice.cpp \
    midiwrapper.cpp \
    mymapcollector.cpp \
    playbackhandler.cpp \
    score.cpp \
    songservice.cpp \
    notemapper.cpp \
    needlemanwunsch.cpp \
    song.cpp \
    statisticsservice.cpp

HEADERS  += events.h \
    clusterhandler.h \
    graphicsscoreitem.h \
    guidoservice.h \
    scoreservice.h \
    hanonsongfactory.h \
    matchinghandler.h \
    matchingservice.h \
    matchingitem.h \
    merginghandler.h \
    midiservice.h \
    midiwrapper.h \
    mididefs.h \
    mymapcollector.h \
    playbackhandler.h \
    score.h \
    songservice.h \
    notemapper.h \
    needlemanwunsch.h \
    song.h \
    statisticsservice.h
