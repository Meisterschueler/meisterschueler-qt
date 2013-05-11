#include "matchingservice.h"

#include "events.h"

MatchingService::MatchingService()
{
}

QByteArray MatchingService::midiEvents2pitchSequence(QList<NoteEventPair> events) {
    QByteArray sequence;
    for (NoteEventPair midiPair : events) {
        sequence.append(midiPair.noteOn->getNote());
    }
    return sequence;
}

QByteArray MatchingService::midiEvents2intervalSequence(QList<NoteEventPair> events) {
    QByteArray sequence;
    QByteArray pitchSequence = midiEvents2pitchSequence(events);
    for (int i=1; i<pitchSequence.length(); i++) {
        char delta = pitchSequence.at(i) - pitchSequence.at(i-1);
        sequence.append(delta);
    }
    return sequence;
}

QByteArray MatchingService::midiEvents2pressedSequence(QList<NoteEventPair> events) {
    QByteArray sequence;
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

char MatchingService::getTransposition(QByteArray midiPitchSequence, QByteArray scorePitchSequence, QByteArray intervalAlignment) {
    int posAlignment = intervalAlignment.lastIndexOf("mmm");
    if (posAlignment < 0) {
        return 0;   // TODO: nicht gut...
    }

    QByteArray goodString = intervalAlignment.mid(0, posAlignment);

    int scorePosition = goodString.replace("i", "").length();
    int notePosition = goodString.replace("d", "").length();

    char scorePitch = scorePitchSequence.at(scorePosition);
    char notePitch = midiPitchSequence.at(notePosition);

    return notePitch - scorePitch;
}

bool MatchingService::isFinished(QByteArray pitchAlignment, QByteArray pressedSequence) {
    return false;
}
