#include "clusterhandler.h"

#include "midiservice.h"

#include <QDebug>
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

    channelEvents.append(noteOn);

    if (!delayTimer->isActive()) {
        delayTimer->start(CHORD_DELAY);
    }
    pressedKeys[noteOn.getNote()] = true;
}

void ClusterHandler::matchNoteOffEvent(NoteOffEvent noteOff) {
    channelEvents.append(noteOff);

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
    QMap<int, MidiPairCluster> modifiedClusters;
    for (ChannelEvent e : channelEvents) {
        if (e.type() == Event::NoteOnEventType) {
            int idx = MidiService::addNoteOn(midiPairClusters, (NoteOnEvent)e);
            modifiedClusters.insert(idx, midiPairClusters.at(idx));
        } else if (e.type() == Event::NoteOffEventType) {
            int idx = MidiService::addNoteOff(midiPairClusters, (NoteOffEvent)e);
            modifiedClusters.insert(idx, midiPairClusters.at(idx));
        }
    }

    for (int idx : modifiedClusters.keys()) {
        emit gotMidiPairCluster(idx, modifiedClusters.value(idx));
    }
    emit gotMidiPairClusters(midiPairClusters);

    channelEvents.clear();
}

void ClusterHandler::timeOutReset() {
    emit reset();
}
