#ifndef SONGSERVICE_H
#define SONGSERVICE_H

#include <QList>

class MatchingItem;

class SongService
{
public:
    SongService();

    QList<MatchingItem> getMatchingItems();
};

#endif // SONGSERVICE_H
