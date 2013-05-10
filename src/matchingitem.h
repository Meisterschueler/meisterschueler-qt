#ifndef MATCHINGITEM_H
#define MATCHINGITEM_H

#include <QByteArray>

#include "song.h"

class MatchingItem
{
public:
    MatchingItem();
    MatchingItem(const Song& song, const QByteArray& noteSequence, const QByteArray& intervalSequence);

    Song song;
    QByteArray noteSequence;
    QByteArray intervalSequence;
};

#endif // MATCHINGITEM_H
