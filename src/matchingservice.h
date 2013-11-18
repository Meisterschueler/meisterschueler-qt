#ifndef MATCHINGSERVICE_H
#define MATCHINGSERVICE_H

#include <QByteArray>
#include <QList>

class Score;
class MidiPair;
class MidiPairCluster;

class MatchingService
{
public:
    MatchingService();

    static const char PRESSED = 'X';
    static const char RELEASED = '.';

    // pre matching
    static QByteArray midiPairClusters2pitchSequence(const QList<MidiPairCluster> &midiPairClusters);
    static QByteArray midiPairClusters2intervalSequence(const QList<MidiPairCluster>& midiPairClusters);
    static QByteArray midiPairClusters2pressedSequence(const QList<MidiPairCluster>& midiPairClusters);

    // matching
    static QByteArray getAlingment(const QByteArray& scorePitchSequence, const QByteArray& midiPitchSequence, const char& transposition=0, const QByteArray& oldAlignment = "");
    static QByteArray getSaveAlignment(const QByteArray& alignment);
    static char getTransposition(const QByteArray& scorePitchSequence, const QByteArray& midiPitchSequence, const QByteArray& intervalAlignment);
    static double getSongQuality(const QByteArray& pitchAlignment, char transposition);
    static double getChordQuality(const QByteArray& pitchAlignment);
    static bool isFinished(const QByteArray& pitchAlignment, const QByteArray& pressedSequence);
    static QList<MidiPairCluster> cutMatchingMidiPairs(QList<MidiPairCluster>& pairs, const QByteArray& pitchAlignment);

    // post matching
    static QList<Score> merge(const QList<Score>& scores, const QList<MidiPairCluster>& midiPairClusters, const QByteArray& pitchAlignment);
};

#endif // MATCHINGSERVICE_H
