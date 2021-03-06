#include "midiwrapper.h"

#include <cmath>
#include <ctime>

#include <QApplication>
#include <QSettings>
#include <QStringList>

#include "RtMidi.h"
#include "mididefs.h"
#include "events.h"

void midiCallback(double deltatime, std::vector<unsigned char> *message,
                  void *userData) {
    QEvent* ev = NULL;
    MidiWrapper* instance = static_cast<MidiWrapper*> (userData);

    static clock_t time = 0;
    time += floor(1000.0 * deltatime);

    unsigned char status = message->at(0) & MASK_STATUS;
    unsigned char channel = message->at(0) & MASK_CHANNEL;
    switch (status) {
    case STATUS_NOTEON:
    case STATUS_NOTEOFF: {
        unsigned char midi_note = message->at(1);
        unsigned char vel = message->at(2);
        if ((status == STATUS_NOTEOFF) || (vel == 0))
            ev = new NoteOffEvent(time, channel, midi_note, vel);
        else
            ev = new NoteOnEvent(time, channel, midi_note, vel);
    }
        break;
    case STATUS_CTLCHG: {
        unsigned char ctl = message->at(1);
        unsigned char val = message->at(2);
        ev = new ControlChangeEvent(time, ctl, val);
    }
        break;
    }
    if (ev != NULL)
        QApplication::postEvent(instance, ev);
}


MidiWrapper::MidiWrapper(QObject *parent) :
    QObject(parent)
{
    try {
        midiIn = new RtMidiIn();
        midiOut = new RtMidiOut();

        QSettings settings;
        QString inputPortName = settings.value("midi/input", tr("(none)")).toString();
        QString outputPortName = settings.value("midi/output", tr("(none)")).toString();

        openInputPort(inputPortName);
        openOutputPort(outputPortName);
    } catch (...) {
        qDebug("WTF! Initialization of the sound failed!");
        midiIn = 0;
        midiOut = 0;
    }
}

MidiWrapper::~MidiWrapper() {
    if (midiIn)
        delete midiIn;

    if (midiOut)
        delete midiOut;
}

QStringList MidiWrapper::getInputPorts() const {
    QStringList result;
    result << tr("(none)");

    if (midiIn) {
        for (unsigned int i = 0; i < midiIn->getPortCount(); i++) {
            QString portName = midiIn->getPortName(i).c_str();
            if (!portName.startsWith("RtMidi"))
                result << portName;
        }
    }

    return result;
}

QStringList MidiWrapper::getOutputPorts() const {
    QStringList result;
    result << tr("(none)");

    if (midiOut) {
        for (unsigned int i = 0; i < midiOut->getPortCount(); i++) {
            QString portName = midiOut->getPortName(i).c_str();
            if (!portName.startsWith("RtMidi"))
                result << portName;
        }
    }

    return result;
}

QString MidiWrapper::getOpenedInputPort() const {
    return openedInputPort;
}

QString MidiWrapper::getOpenedOutputPort() const {
    return openedOutputPort;
}

void MidiWrapper::customEvent(QEvent *event) {
    if (event->type() == Event::NoteOnEventType) {
        NoteOnEvent *ev = static_cast<NoteOnEvent*> (event);
        emit gotNoteOnEvent(*ev);
    } else if (event->type() == Event::NoteOffEventType) {
        NoteOffEvent *ev = static_cast<NoteOffEvent*> (event);
        emit gotNoteOffEvent(*ev);
    } else if (event->type() == Event::ControlChangeEventType) {
        ControlChangeEvent *ev = static_cast<ControlChangeEvent*> (event);
        emit gotControlChangeEvent(*ev);
    }
    event->accept();
}

void MidiWrapper::playNoteOnEvent(NoteOnEvent event) {
    if (!openedOutputPort.isEmpty() && openedOutputPort != tr("(none)")) {
        std::vector<unsigned char> message;
        message.push_back(STATUS_NOTEON);
        message.push_back(event.getNote());
        message.push_back(event.getVelocity());
        midiOut->sendMessage(&message);
    }
}

void MidiWrapper::playNoteOffEvent(NoteOffEvent event) {
    if (!openedOutputPort.isEmpty() && openedOutputPort != tr("(none)")) {
        std::vector<unsigned char> message;
        message.push_back(STATUS_NOTEOFF);
        message.push_back(event.getNote());
        message.push_back(event.getVelocity());
        midiOut->sendMessage(&message);
    }
}

void MidiWrapper::openInputPort(QString portName) {
    if (!openedInputPort.isEmpty() && openedInputPort != tr("(none)")) {
        midiIn->closePort();
    }

    int port = getInputPorts().indexOf(portName) - 1;
    if (port >= 0) {
        midiIn->openPort(port);
        midiIn->setCallback(&midiCallback, this);
        midiIn->ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages.
    }
    QSettings settings;
    settings.setValue("midi/input", portName);
    openedInputPort = portName;
}

void MidiWrapper::openOutputPort(QString portName) {
    if (!openedOutputPort.isEmpty() && openedOutputPort != tr("(none)")) {
        midiOut->closePort();
    }

    int port = getOutputPorts().indexOf(portName) - 1;
    if (port >= 0) {
        midiOut->openPort(port);
    }
    QSettings settings;
    settings.setValue("midi/output", portName);
    openedOutputPort = portName;
}
