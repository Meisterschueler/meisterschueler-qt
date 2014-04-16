#Use pre-compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER = ../stable.h
OTHER_FILES += ../stable.h

#Use C++11
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

#OSX stuff...
mac {
    QMAKE_CXXFLAGS = -mmacosx-version-min=10.7
}

#Path definitions
GUIDOLIB = ../../guidolib-code
RTMIDI = ../../rtmidi-2.1.0
JDKSMIDI = ../../jdksmidi
KISSFFT = ../../kiss_fft130
QCUSTOMPLOT = ../../qcustomplot

#Guido
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
INCLUDEPATH += $$GUIDOLIB/Qt/GuidoQt/include \
               $$GUIDOLIB/src/include \
               $$GUIDOLIB/src/misc \
               $$GUIDOLIB/src/lib \
               $$GUIDOLIB/src/abstract \
               $$GUIDOLIB/src/parser \
               $$GUIDOLIB/src/midisharelight

LIBS += -L$$GUIDOLIB/Qt \
        -L$$GUIDOLIB/cmake \
        -lGuidoQt \
        -lGUIDOEngine

#RtMidi
INCLUDEPATH += $$RTMIDI
SOURCES += $$RTMIDI/RtMidi.cpp

debug {
    DEFINES += __RTMIDI_DEBUG__
}

unix {
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
