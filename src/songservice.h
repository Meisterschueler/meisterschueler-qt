#ifndef SONGSERVICE_H
#define SONGSERVICE_H

#include <QList>

class MatchingItem;

class SongService
{
public:
    SongService();

    static QList<MatchingItem> getMatchingItems();
};

#endif // SONGSERVICE_H
