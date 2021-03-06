#ifndef MATCHINGITEM_H
#define MATCHINGITEM_H

#include <QByteArray>
#include <QSharedPointer>

#include "events.h"
#include "song.h"

class MatchingItem
{
public:
    MatchingItem();
    MatchingItem(const Song& song, const QByteArray& scorePitchSequence, const QByteArray& scoreIntervalSequence);

    bool enabled = true;

    Song song;
    QByteArray scorePitchSequence;
    QByteArray scoreIntervalSequence;

    double quality;
    char transposition;

    QSharedPointer<QList<MidiPairCluster>> midiPairClusters;
    QSharedPointer<QByteArray> midiPitchSequence;
    QSharedPointer<QByteArray> midiIntervalSequence;
    QSharedPointer<QByteArray> pressedSequence;

    QByteArray pitchAlignment;
    QByteArray intervalAlignment;

    QList<Score> mergedScores;

    bool operator < (const MatchingItem& rhs) const {
        return (this->quality > rhs.quality);
    }

    bool operator > (const MatchingItem& rhs) const {
        return (this->quality < rhs.quality);
    }
};
Q_DECLARE_METATYPE(MatchingItem)

#endif // MATCHINGITEM_H
