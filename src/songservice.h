#ifndef SONGSERVICE_H
#define SONGSERVICE_H

#include <QList>
#include <QStringList>

#include "score.h"

class MatchingItem;
class Song;

class SongService
{
public:
    SongService();

    static QList<Song> getSongsBuiltIn();
    static QList<Song> getSongsFromDirectory(const QString &directory);

    static QList<MatchingItem> createMatchingItems(const QList<Song> &songs);

    static int countSpecialInterval(const Song &song, const int &interval, const Finger &fingerFrom, const Finger &fingerTo);

private:
    static QStringList getGmnFileNames(const QString& directory);
};

#endif // SONGSERVICE_H
