#Use pre-compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER = ../stable.h
OTHER_FILES += ../stable.h

#Use C++11
QMAKE_CXXFLAGS += -std=c++0x

#Path definitions
GUIDOLIB = ../../guidolib-code
RTMIDI = ../../rtmidi-2.0.1
JDKSMIDI = ../../jdksmidi
KISSFFT = ../../kiss_fft130

#Guido
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

#RtMidi
INCLUDEPATH += $$RTMIDI

LIBS += -L$$RTMIDI \
        -lrtmidi

unix {
    DEFINES += __UNIX_JACK__
    LIBS += -ljack
#    DEFINES += __LINUX_ALSASEQ__
#    LIBS += -lasound \
#            -pthread
}

win32 {
    DEFINES += __WINDOWS_MM__
    LIBS += -lwinmm \
            -lWsock32
}

mac {
    DEFINES += __MACOSX_CORE__
    LIBS += -framework CoreMidi \
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

HEADERS +=
