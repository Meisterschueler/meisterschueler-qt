#ifndef SONGSERVICE_H
#define SONGSERVICE_H

#include <QList>
#include <QStringList>

class MatchingItem;
class Song;

class SongService
{
public:
    SongService();

    static QList<Song> getSongsBuiltIn();
    static QList<Song> getSongsFromDirectory(const QString &directory);

    static QList<MatchingItem> createMatchingItems(const QList<Song> &songs);

private:
    static QStringList getGmnFileNames(const QString& directory);
};

#endif // SONGSERVICE_H
