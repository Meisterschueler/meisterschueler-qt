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
DEPENDPATH += ../src
INCLUDEPATH += ../src

SOURCES += guidoservice.cpp \
    scoreservice.cpp \
    score.cpp \
    notemapper.cpp \
    needlemanwunsch.cpp

HEADERS  += guidoservice.h \
    scoreservice.h \
    events.h \
    mididefs.h \
    score.h \
    notemapper.h \
    needlemanwunsch.h
