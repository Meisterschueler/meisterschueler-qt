#include "clusterhandler.h"

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
        delayTimer->start(50);
    }
    pressedKeys[noteOn.getNote()] = true;
}

void ClusterHandler::matchNoteOffEvent(NoteOffEvent noteOff) {    
    channelEvents.append(noteOff);
    if (!delayTimer->isActive()) {
        delayTimer->start(50);
    }
    pressedKeys[noteOff.getNote()] = false;

    bool anyKeyPressed = false;

    for (bool key : pressedKeys) {
        if (key) {
            anyKeyPressed = true;
        }
    }

    if (!anyKeyPressed) {
        resetTimer->start(2000);
    }
}

void ClusterHandler::timeOutDelay() {
    emit gotChannelEvents(channelEvents);
    channelEvents.clear();
}

void ClusterHandler::timeOutReset() {
    emit reset();
}
