#-------------------------------------------------
#
# Project created by QtCreator 2012-12-21T13:05:39
#
#-------------------------------------------------

include(../common.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = src
TEMPLATE = app

SOURCES += main.cpp\
    guidoservice.cpp \
    midiwrapper.cpp \
    notemapper.cpp \
    song.cpp \
    needlemanwunsch.cpp \
    matchinghandler.cpp \
    matchingitem.cpp \
    matchingservice.cpp \
    hanonsongfactory.cpp \
    scoreservice.cpp \
    score.cpp \
    midiservice.cpp \
    songservice.cpp \
    statisticsservice.cpp

HEADERS  += \
    guidoservice.h \
    midiwrapper.h \
    mididefs.h \
    notemapper.h \
    song.h \
    needlemanwunsch.h \
    events.h \
    matchinghandler.h \
    matchingitem.h \
    matchingservice.h \
    hanonsongfactory.h \
    scoreservice.h \
    score.h \
    midiservice.h \
    util.h \
    songservice.h \
    statisticsservice.h

FORMS    +=

OTHER_FILES +=

