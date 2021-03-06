#include "matchingservice.h"

#include <QRegExp>

#include "events.h"
#include "needlemanwunsch.h"
#include "score.h"
#include "util.h"

MatchingService::MatchingService()
{
}

QByteArray MatchingService::midiPairClusters2pitchSequence(const QList<MidiPairCluster>& midiPairClusters) {
    QByteArray sequence;
    for (MidiPairCluster midiPairCluster : midiPairClusters) {
        for (MidiPair midiPair : midiPairCluster.midiPairs) {
            sequence.append(midiPair.noteOn.getNote());
        }
    }
    return sequence;
}

QByteArray MatchingService::midiPairClusters2intervalSequence(const QList<MidiPairCluster>& midiPairClusters) {
    QByteArray sequence;
    QByteArray pitchSequence = midiPairClusters2pitchSequence(midiPairClusters);
    for (int i=1; i<pitchSequence.length(); i++) {
        char delta = pitchSequence.at(i) - pitchSequence.at(i-1);
        sequence.append(delta);
    }
    return sequence;
}

QByteArray MatchingService::midiPairClusters2pressedSequence(const QList<MidiPairCluster> &midiPairClusters) {
    QByteArray sequence;
    for (MidiPairCluster midiPairCluster : midiPairClusters) {
        for (MidiPair midiPair : midiPairCluster.midiPairs) {
            if (midiPair.noteOn != emptyNoteOnEvent && midiPair.noteOff != emptyNoteOffEvent) {
                sequence.append(MatchingService::RELEASED);
            } else if (midiPair.noteOn != emptyNoteOnEvent){
                sequence.append(MatchingService::PRESSED);
            } else if (midiPair.noteOff != emptyNoteOffEvent) {
                sequence.append("y");
            } else {
                sequence.append("o");
            }
        }
    }
    return sequence;
}

QByteArray MatchingService::getAlingment(const QByteArray& scorePitchSequence, const QByteArray& midiPitchSequence, const char& transposition, const QByteArray& saveAlignment) {
    NeedlemanWunsch needlemanWunsch;

    if (saveAlignment.isEmpty()) {
        return needlemanWunsch.getAlignments(scorePitchSequence, midiPitchSequence, transposition);
    } else {
        QByteArray saveAlignment_temp1(saveAlignment);
        QByteArray saveAlignment_temp2(saveAlignment);
        QByteArray prunnedScorePitchSequence = scorePitchSequence.mid(saveAlignment_temp1.replace("i", "").length());
        QByteArray prunnedMidiPitchSequence = midiPitchSequence.mid(saveAlignment_temp2.replace("d", "").length());

        QByteArray prunnedAlignment = needlemanWunsch.getAlignments(prunnedScorePitchSequence, prunnedMidiPitchSequence, transposition);

        return saveAlignment + prunnedAlignment;
    }
}

QByteArray MatchingService::getSaveAlignment(const QByteArray &alignment) {
    int saveRegion = alignment.lastIndexOf("mmmmmmmmmm");
    if (saveRegion >= 0) {
        return alignment.mid(0, saveRegion);
    } else {
        return "";
    }
}

char MatchingService::getTransposition(const QByteArray& scorePitchSequence, const QByteArray& midiPitchSequence, const QByteArray& intervalAlignment) {
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

double MatchingService::getSongQuality(const QByteArray &pitchAlignment, char transposition) {
    QString alignment(pitchAlignment);

    int firstHitChord = alignment.replace(QRegExp("[mwi]"), ".").indexOf(".");
    int lastHitChord = alignment.replace(QRegExp("[mwi]"), ".").lastIndexOf(".");

    int open = pitchAlignment.count(NeedlemanWunsch::OPEN);
    int played = pitchAlignment.count(NeedlemanWunsch::MATCH);
    //int missed = pitchAlignment.count(NeedlemanWunsch::DELETED);
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

double MatchingService::getChordQuality(const QByteArray &pitchAlignment) {
    //int open = pitchAlignment.count(NeedlemanWunsch::OPEN);
    int played = pitchAlignment.count(NeedlemanWunsch::MATCH);
    //int missed = pitchAlignment.count(NeedlemanWunsch::DELETED);
    //int extra = pitchAlignment.count(NeedlemanWunsch::INSERT);
    //int wrong = pitchAlignment.count(NeedlemanWunsch::WRONG);

    return (double)played/pitchAlignment.size();
}

bool MatchingService::isFinished(const QByteArray& pitchAlignment, const QByteArray& pressedSequence) {
    bool result = false;
    QString alignment(pitchAlignment);
    while (alignment.endsWith("i")) {
        alignment = alignment.mid(0, alignment.length()-1);
    }
    if (alignment.endsWith("m")) {
        alignment.replace("d", "");
        int idxPressed = pressedSequence.indexOf(MatchingService::PRESSED);
        if (idxPressed != -1 && idxPressed < alignment.length()) {
            result = false;
        } else {
            result = true;
        }
    } else {
        result = false;
    }

    return result;
}

QList<MidiPairCluster> MatchingService::cutMatchingMidiPairs(QList<MidiPairCluster>& pairs, const QByteArray& pitchAlignment) {
    QList<MidiPairCluster> result;

    QString alignment(pitchAlignment);
    while (alignment.endsWith("i")) {
        alignment.remove(alignment.size()-1, 1);
    }
    alignment.remove('d');

    int idx = 0;
    int splitpoint = alignment.size();
    for (int i = 0; i < pairs.count(); ++i) {
        idx += pairs.at(i).midiPairs.count();
        if (idx > splitpoint) {
            result = pairs.mid(i);
            pairs = pairs.mid(0, i);
        }
    }

    return result;
}

QList<Score> MatchingService::merge(const QList<Score>& scores, const QList<MidiPairCluster>& midiPairClusters, const QByteArray& pitchAlignment) {
    QList<MidiPair> midiPairs;
    for (MidiPairCluster mpc : midiPairClusters) {
        for (MidiPair midiPair : mpc.midiPairs) {
            midiPairs.append(midiPair);
        }
    }

    QList<Score> result;
    int idx_scores = 0, idx_midiPairs = 0, idx_alignment = 0;
    while (idx_alignment < pitchAlignment.size()) {
        switch (pitchAlignment.at(idx_alignment)) {
        case 'm': {
            Score s = scores.at(idx_scores);
            s.midiPair = midiPairs.at(idx_midiPairs);
            s.status = PLAYED;
            result.append(s);
            idx_scores++;
            idx_midiPairs++;
            idx_alignment++;
            break;
        }
        case 'w': {
            Score s = scores.at(idx_scores);
            s.midiPair = midiPairs.at(idx_midiPairs);
            s.status = FAILED;
            result.append(s);
            idx_scores++;
            idx_midiPairs++;
            idx_alignment++;
            break;
        }
        case 'i': {
            Score s(midiPairs.at(idx_midiPairs).noteOn.getNote());
            s.midiPair = midiPairs.at(idx_midiPairs);
            s.status = EXTRA;
            result.append(s);
            idx_midiPairs++;
            idx_alignment++;
            break;
        }
        case 'd': {
            Score s = scores.at(idx_scores);
            s.status = MISSED;
            result.append(s);
            idx_scores++;
            idx_alignment++;
            break;
        }
        }
    }
    return result;
}
