#System minimum
lessThan(QT_MAJOR_VERSION, 5) {
    message("Meisterschueler requires QT >= 5 to work")
}
mac {
    QMAKE_CXXFLAGS = -mmacosx-version-min=10.7
}

#Version information
#REVISION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe)
#REVISION = $$system(git rev-parse HEAD | cut -c1-10)
#DEFINES += GITHASH=\\\"$$REVISION\\\"
DEFINES += VER=\\\"asdf\\\"

#Use pre-compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER = ../stable.h
OTHER_FILES += ../stable.h

#Use C++11
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

#Path definitions
mac: MAINPATH = /Users/konstantin/Development/Meisterschueler
DEFINES += MAINPATH=\\\"$$MAINPATH\\\"
GUIDOLIB = $$MAINPATH/guidolib-code
RTMIDI = $$MAINPATH/rtmidi-2.1.0
JDKSMIDI = $$MAINPATH/jdksmidi
KISSFFT = $$MAINPATH/kiss_fft130
QCUSTOMPLOT = $$MAINPATH/qcustomplot

#Guido
INCLUDEPATH += $$GUIDOLIB/src/include \
               $$GUIDOLIB/src/misc \
               $$GUIDOLIB/src/lib \
               $$GUIDOLIB/src/abstract \
               $$GUIDOLIB/src/parser \
               $$GUIDOLIB/src/midisharelight

win32,unix,!mac: LIBS += -L$$GUIDOLIB/cmake -lGUIDOEngine
mac:LIBS += -F$$GUIDOLIB/cmake/Debug -framework GUIDOEngine

#GuidoQt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
INCLUDEPATH += $$GUIDOLIB/Qt/GuidoQt/include

LIBS += -L$$GUIDOLIB/Qt -lGuidoQt

#RtMidi
INCLUDEPATH += $$RTMIDI
SOURCES += $$RTMIDI/RtMidi.cpp

debug {
    DEFINES += __RTMIDI_DEBUG__
}

unix,!mac {
# Ensure jack is running: jackd -d alsa -X raw
#    DEFINES += __UNIX_JACK__
#    LIBS += -ljack
    DEFINES += __LINUX_ALSA__
    LIBS += -lasound \
            -lpthread
}

win32 {
    DEFINES += __WINDOWS_MM__
    LIBS += -lwinmm \
            -lWsock32
}

mac {
    DEFINES += __MACOSX_CORE__
    LIBS += -framework CoreMIDI \
            -framework CoreAudio \
            -framework CoreFoundation
}

#jdksmidi
INCLUDEPATH += $$JDKSMIDI/include

LIBS += -L$$JDKSMIDI \
        -ljdksmidi

#kissfft
INCLUDEPATH += $$KISSFFT
SOURCES += $$KISSFFT/kiss_fft.c \
           $$KISSFFT/tools/kiss_fftr.c

#QCustomPlot
INCLUDEPATH += $$QCUSTOMPLOT
SOURCES += $$QCUSTOMPLOT/qcustomplot.cpp
HEADERS += $$QCUSTOMPLOT/qcustomplot.h
