#-------------------------------------------------
#
# Project created by QtCreator 2013-06-04T12:15:18
#
#-------------------------------------------------

#Use C++11
QMAKE_CXXFLAGS += -std=c++0x

#Guido
GUIDOLIB = ../../guidolib-code
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
INCLUDEPATH += $$GUIDOLIB/Qt/GuidoQt/include \
               $$GUIDOLIB/src/include \
               $$GUIDOLIB/src/misc \
               $$GUIDOLIB/src/lib \
               $$GUIDOLIB/src/abstract \
               $$GUIDOLIB/src/parser \
               $$GUIDOLIB/src/midisharelight

LIBS += -L$$GUIDOLIB/Qt/GuidoQt \
        -L$$GUIDOLIB/cmake \
        -lGuidoQt \
        -lGUIDOEngine

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = minimal
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

FORMS    +=
