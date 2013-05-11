#include "matchingservice.h"

#include <QRegExp>

#include "events.h"
#include "needlemanwunsch.h"
#include "util.h"

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

double MatchingService::getQuality(QByteArray pitchAlignment, char transposition) {
    QString alignment(pitchAlignment);

    int firstHitChord = alignment.replace(QRegExp("[mwi]"), ".").indexOf(".");
    int lastHitChord = alignment.replace(QRegExp("[mwi]"), ".").lastIndexOf(".");

    int open = pitchAlignment.count(NeedlemanWunsch::OPEN);
    int played = pitchAlignment.count(NeedlemanWunsch::MATCH);
    int missed = pitchAlignment.count(NeedlemanWunsch::DELETED);
    int extra = pitchAlignment.count(NeedlemanWunsch::INSERT);
    int wrong = pitchAlignment.count(NeedlemanWunsch::WRONG);

    double rangeFactor = (double)(played+wrong+extra)/(double)(lastHitChord - firstHitChord + 1);
    double progressFactor = (double)(played+wrong)/(double)(played+wrong+extra+open);
    double matchFactor = (double)(played)/(double)(played+wrong+extra);

    double transpositionFactor;
    if (transposition >= 0) {
        transpositionFactor = 1.0 - transposition/1100.0;
    } else {
        transpositionFactor = 0.99 + transposition/1100.0;
    }

    double quality = rangeFactor*matchFactor*MAX2(0.9, progressFactor)*transpositionFactor;

    if (std::isinf(quality) || std::isnan(quality)) {
        return 0.0;
    } else {
        return quality;
    }
}

bool MatchingService::isFinished(QByteArray pitchAlignment, QByteArray pressedSequence) {
    bool result = false;
    while (pitchAlignment.endsWith("i")) {
        pitchAlignment = pitchAlignment.mid(0, pitchAlignment.length()-1);
    }
    if (pitchAlignment.endsWith("m")) {
        pitchAlignment.replace("d", "");
        int idxPressed = pressedSequence.indexOf("D");
        if (idxPressed != -1 && idxPressed < pitchAlignment.length()) {
            result = false;
        } else {
            result = true;
        }
    } else {
        result = false;
    }

    return result;
}

QList<NoteEventPair> MatchingService::cutMatchingMidiEvents(QList<NoteEventPair> events, QByteArray pitchAlignment) {
    QList<NoteEventPair> result;

    QString alignment(pitchAlignment);
    while (alignment.endsWith("i")) {
        alignment.remove(alignment.size()-1, 1);
    }
    alignment.remove('d');

    result = events.mid(alignment.size());
    events = events.mid(0, alignment.size());

    return result;
}
