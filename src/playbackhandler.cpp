#include "playbackhandler.h"

#include <QTimer>

PlaybackHandler::PlaybackHandler(QObject *parent) :
    QObject(parent)
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, this, &PlaybackHandler::playNextEvent);
}

void PlaybackHandler::setMidiPairs(QList<MidiPair> midiPairs) {
    idx = 0;
    events.clear();
    for (MidiPair midiPair : midiPairs) {
        events.append(midiPair.noteOn);
        events.append(midiPair.noteOff);
    }

    qSort(events);
}

void PlaybackHandler::setChannelEvents(QList<ChannelEvent> channelEvents) {
    events = channelEvents;
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
        idx++;
        timer->start(delta);
    }
}
