#include "playbackhandler.h"

PlaybackHandler::PlaybackHandler(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(playNextEvent()));
}

void PlaybackHandler::setMidiPairs(QList<MidiPair> midiPairs) {
    idx = 0;
    events.clear();
    for (MidiPair midiPair : midiPairs) {
        events.append(*midiPair.noteOn);
        events.append(*midiPair.noteOff);
    }

    qSort(events);
}

void PlaybackHandler::play() {
    if (idx<events.size()) {
        timer->start(0);
    }
}

void PlaybackHandler::stop() {
    timer->stop();
}

void PlaybackHandler::jumpTo(Fraction position) {

}

void PlaybackHandler::playNextEvent() {
    if (events.at(idx).type() == Event::NoteOnEventType) {
        emit gotNoteOnEvent(NoteOnEvent(events.at(idx)));
    } else if (events.at(idx).type() == Event::NoteOffEventType) {
        emit gotNoteOffEvent(NoteOffEvent(events.at(idx)));
    }

    if (idx+1 < events.size()) {
        int delta = events.at(idx+1).getTime() - events.at(idx).getTime();
        timer->start(delta);
    }
}