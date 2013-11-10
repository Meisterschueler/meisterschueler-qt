#include "clusterhandler.h"

#include "midiservice.h"

#include <QTimer>

ClusterHandler::ClusterHandler()
{
    delayTimer = new QTimer(this);
    delayTimer->setSingleShot(true);
    QObject::connect(delayTimer, &QTimer::timeout, this, &ClusterHandler::timeOutDelay);

    resetTimer = new QTimer(this);
    resetTimer->setSingleShot(true);
    QObject::connect(resetTimer, &QTimer::timeout, this, &ClusterHandler::timeOutReset);

    for (int i = 0; i < 128; ++i) {
        pressedKeys[i] = false;
    }
}

void ClusterHandler::matchNoteOnEvent(NoteOnEvent noteOn) {
    resetTimer->stop();

    MidiService::addNoteOn(midiPairs, noteOn);
    emit gotMidiPairs(midiPairs);
    channelEvents.append(noteOn);
    if (!delayTimer->isActive()) {
        delayTimer->start(CHORD_DELAY);
    }
    pressedKeys[noteOn.getNote()] = true;
}

void ClusterHandler::matchNoteOffEvent(NoteOffEvent noteOff) {    
    MidiService::addNoteOff(midiPairs, noteOff);
    channelEvents.append(noteOff);
    emit gotMidiPairs(midiPairs);
    if (!delayTimer->isActive()) {
        delayTimer->start(CHORD_DELAY);
    }
    pressedKeys[noteOff.getNote()] = false;

    bool anyKeyPressed = false;

    for (bool key : pressedKeys) {
        if (key) {
            anyKeyPressed = true;
        }
    }

    if (!anyKeyPressed) {
        resetTimer->start(RESET_DELAY);
    }
}

void ClusterHandler::timeOutDelay() {
    emit gotChannelEvents(channelEvents);
    channelEvents.clear();
}

void ClusterHandler::timeOutReset() {
    midiPairs.clear();
    emit reset();
}
