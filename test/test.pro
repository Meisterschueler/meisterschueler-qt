#-------------------------------------------------
#
# Project created by QtCreator 2012-12-21T13:04:39
#
#-------------------------------------------------

include(../common.pri)

QT       += testlib

QT       -= gui

TARGET = tst_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

# Get Classes
INCLUDEPATH += ../src
VPATH += ../src

SOURCES += chordhandler.cpp \
    events.cpp \
    clusterhandler.cpp \
    guidoservice.cpp \
    scoreservice.cpp \
    hanonsongfactory.cpp \
    matchinghandler.cpp \
    matchingservice.cpp \
    matchingitem.cpp \
    merginghandler.cpp \
    midiservice.cpp \
    midiwrapper.cpp \
    playbackhandler.cpp \
    recordhandler.cpp \
    score.cpp \
    songservice.cpp \
    notemapper.cpp \
    needlemanwunsch.cpp \
    song.cpp \
    statisticsservice.cpp

HEADERS  += chordhandler.h \
    events.h \
    clusterhandler.h \
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
    playbackhandler.h \
    recordhandler.h \
    score.h \
    songservice.h \
    notemapper.h \
    needlemanwunsch.h \
    song.h \
    statisticsservice.h

OTHER_FILES += \
    midifiles/hanonNo1Left.mid \
    midifiles/hanonNo1Both.mid
