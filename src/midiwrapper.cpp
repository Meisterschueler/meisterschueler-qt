#include "midiwrapper.h"

#include "math.h"

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
    midiIn = new RtMidiIn();
    midiOut = new RtMidiOut();
}

MidiWrapper::~MidiWrapper() {
    delete midiIn;
    delete midiOut;
}

QStringList MidiWrapper::getInputPorts() {
    QStringList result;

    for (unsigned int i = 0; i < midiIn->getPortCount(); i++) {
        QString portName = midiIn->getPortName(i).c_str();
        if (!portName.startsWith("RtMidi"))
            result << portName;
    }

    return result;
}

QStringList MidiWrapper::getOutputPorts() {
    QStringList result;

    for (unsigned int i = 0; i < midiOut->getPortCount(); i++) {
        QString portName = midiOut->getPortName(i).c_str();
        if (!portName.startsWith("RtMidi"))
            result << portName;
    }

    return result;
}

QString MidiWrapper::getOpenedInputPort() {
    return openedInputPort;
}

QString MidiWrapper::getOpenedOutputPort() {
    return openedOutputPort;
}

void MidiWrapper::customEvent(QEvent *event) {
    if (event->type() == NoteOnEventType) {
        NoteOnEvent *ev = static_cast<NoteOnEvent*> (event);
        emit noteOn(*ev);
    } else if (event->type() == NoteOffEventType) {
        NoteOffEvent *ev = static_cast<NoteOffEvent*> (event);
        emit noteOff(*ev);
    } else if (event->type() == ControlChangeEventType) {
        ControlChangeEvent *ev = static_cast<ControlChangeEvent*> (event);
        emit controlChangeEvent(*ev);
    }
    event->accept();
}

void MidiWrapper::noteOn(NoteOnEvent event) {
    if (!openedOutputPort.isEmpty()) {
        std::vector<unsigned char> message;
        message.push_back(STATUS_NOTEON);
        message.push_back(event.getNote());
        message.push_back(event.getVelocity());
        midiOut->sendMessage(&message);
    }
}

void MidiWrapper::noteOff(NoteOffEvent event) {
    if (!openedOutputPort.isEmpty()) {
        std::vector<unsigned char> message;
        message.push_back(STATUS_NOTEOFF);
        message.push_back(event.getNote());
        message.push_back(event.getVelocity());
        midiOut->sendMessage(&message);
    }
}

void MidiWrapper::openInputPort(QString portName) {
    if (!openedInputPort.isEmpty()) {
        midiIn->closePort();
    }

    int port = getInputPorts().indexOf(portName);
    if (port != -1) {
        midiIn->openPort(port);
        midiIn->setCallback(&midiCallback, this);
        midiIn->ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages.
        QSettings settings;
        settings.setValue("midi/input", portName);
    }
    openedInputPort = portName;
}

void MidiWrapper::openOutputPort(QString portName) {
    if (!openedOutputPort.isEmpty()) {
        midiOut->closePort();
    }

    int port = getOutputPorts().indexOf(portName);
    if (port != -1) {
        midiOut->openPort(port);
        QSettings settings;
        settings.setValue("midi/output", portName);
    }
    openedOutputPort = portName;
}
