#include "midiservice.h"

#include "events.h"
#include <QSharedPointer>
#include <QMutableListIterator>

MidiService::MidiService()
{
}

void MidiService::addNoteOn(QList<NoteEventPair>& pairs, NoteOnEvent noteOn) {
    QSharedPointer<NoteOnEvent> pointer(new NoteOnEvent(noteOn));
    NoteEventPair pair;
    pair.noteOn = pointer;
    pairs.append(pair);
    qSort(pairs);
}

void MidiService::addNoteOff(QList<NoteEventPair>& pairs, NoteOffEvent noteOff) {
    QSharedPointer<NoteOffEvent> pointer(new NoteOffEvent(noteOff));
    QMutableListIterator<NoteEventPair> it(pairs);
    bool finished = false;
    while (!finished || it.hasNext()) {
        it.next();
        if (it.value().noteOn->getNote() == pointer->getNote() && !it.value().noteOff) {
            it.value().noteOff = pointer;
            finished = true;
        }
    }
}
