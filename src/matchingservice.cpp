#include "matchingservice.h"

#include "events.h"

MatchingService::MatchingService()
{
}

QString MatchingService::midiEvents2pitchSequence(QList<NoteEventPair> events) {
    QString sequence;
    for (NoteEventPair midiPair : events) {
        sequence.append(midiPair.noteOn->getNote());
    }
    return sequence;
}

QByteArray MatchingService::midiEvents2intervalSequence(QList<NoteEventPair> events) {
    QByteArray sequence;
    QString pitchSequence = midiEvents2pitchSequence(events);
    for (int i=1; i<pitchSequence.length(); i++) {
        char delta = pitchSequence.at(i).toLatin1() - pitchSequence.at(i-1).toLatin1();
        sequence.append(delta);
    }
    return sequence;
}

QString MatchingService::midiEvents2pressedSequence(QList<NoteEventPair> events) {
    QString sequence;
    for (NoteEventPair midiPair : events) {
        if (midiPair.noteOn != NULL && midiPair.noteOff != NULL) {
            sequence.append(".");
        } else if (midiPair.noteOn != NULL){
            sequence.append("X");
        } else if (midiPair.noteOff != NULL) {
            sequence.append("y");
        } else {
            sequence.append("o");
        }
    }
    return sequence;
}
