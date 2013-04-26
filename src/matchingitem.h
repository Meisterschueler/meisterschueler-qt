#ifndef MATCHINGITEM_H
#define MATCHINGITEM_H

#include <QString>

#include "song.h"

class MatchingItem
{
public:
    MatchingItem(const Song& song, const QString& noteSequence, const QString& intervalSequence);

private:
    const Song song;
    const QString noteSequence;
    const QString intervalSequence;
};

#endif // MATCHINGITEM_H
