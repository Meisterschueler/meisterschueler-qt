#include "matchingitem.h"

MatchingItem::MatchingItem() {

}

MatchingItem::MatchingItem(const Song& song,
                           const QByteArray& noteSequence,
                           const QByteArray& intervalSequence) : song(song), noteSequence(noteSequence), intervalSequence(intervalSequence)
{
}
