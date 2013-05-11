#include "matchingitem.h"

MatchingItem::MatchingItem() {

}

MatchingItem::MatchingItem(const Song& song,
                           const QByteArray& noteSequence,
                           const QByteArray& intervalSequence) : song(song), scorePitchSequence(noteSequence), scoreIntervalSequence(intervalSequence)
{
}
