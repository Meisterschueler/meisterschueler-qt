#include "matchingservice.h"

#include "events.h"

MatchingService::MatchingService()
{
}

QString MatchingService::midiEvents2pitchSequence(QList<MidiPair> events) {
    QString sequence;
    for (MidiPair midiPair : events) {
        sequence.append(midiPair.noteOn.getNote());
    }
    return sequence;
}

QString MatchingService::midiEvents2intervalSequence(QList<MidiPair> events) {
    QString sequence;
    QString pitchSequence = midiEvents2pitchSequence(events);
    for (int i=1; i<pitchSequence.length(); i++) {
        sequence.append(pitchSequence.at(i).toAscii() - pitchSequence.at(i-1).toAscii());
    }
    return sequence;
}

QString MatchingService::midiEvents2pressedSequence(QList<MidiPair> events) {
    QString sequence;
    for (MidiPair midiPair : events) {
//        if (midiPair.noteOn != NULL && midiPair.noteOff == NULL) {
//            sequence.append("X");
//        } else {
//            sequence.append(".");
//        }
    }
    return sequence;
}
