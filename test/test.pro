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

SOURCES += guidoservice.cpp \
    scoreservice.cpp \
    midiservice.cpp \
    score.cpp \
    notemapper.cpp \
    needlemanwunsch.cpp

HEADERS  += guidoservice.h \
    scoreservice.h \
    midiservice.h \
    events.h \
    mididefs.h \
    score.h \
    notemapper.h \
    needlemanwunsch.h

OTHER_FILES += \
    midifiles/hanonNo1Left.mid \
    midifiles/hanonNo1Both.mid
