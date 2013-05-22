#-------------------------------------------------
#
# Project created by QtCreator 2012-12-21T13:04:39
#
#-------------------------------------------------

include(../common.pri)

QT       += testlib

QT       -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tst_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += tst_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

# Get Classes
INCLUDEPATH += ../src
VPATH += ../src

SOURCES += guidoservice.cpp \
    scoreservice.cpp \
    hanonsongfactory.cpp \
    matchinghandler.cpp \
    matchingservice.cpp \
    matchingitem.cpp \
    midiservice.cpp \
    midiwrapper.cpp \
    score.cpp \
    songservice.cpp \
    notemapper.cpp \
    needlemanwunsch.cpp \
    song.cpp

HEADERS  += guidoservice.h \
    scoreservice.h \
    hanonsongfactory.h \
    matchinghandler.h \
    matchingservice.h \
    matchingitem.h \
    midiservice.h \
    midiwrapper.h \
    events.h \
    mididefs.h \
    score.h \
    songservice.h \
    notemapper.h \
    needlemanwunsch.h \
    song.h

OTHER_FILES += \
    midifiles/hanonNo1Left.mid \
    midifiles/hanonNo1Both.mid
