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

    Song song;
    QByteArray scorePitchSequence;
    QByteArray scoreIntervalSequence;

    double quality;
    char transposition;

    QSharedPointer<QList<NoteEventPair>> midiEvents;
    QSharedPointer<QByteArray> midiPitchSequence;
    QSharedPointer<QByteArray> midiIntervalSequence;
    QSharedPointer<QByteArray> pressedSequence;

    QByteArray pitchAlignment;
    QByteArray intervalAlignment;

    bool operator<(const MatchingItem& rhs) const {
        if (this->quality > rhs.quality) {
            return true;
        } else {
            return false;
        }
    }

    bool operator>(const MatchingItem& rhs) const {
        if (this->quality < rhs.quality) {
            return true;
        } else {
            return false;
        }
    }
};

#endif // MATCHINGITEM_H
