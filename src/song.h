#ifndef SONG_H
#define SONG_H

#include <QMap>
#include <QList>
#include <QString>

#include "score.h"

class Song
{
public:
    Song();

    unsigned long long int id;
    QString name;
    QString description;
    QString gmn;

    QString pattern;
    QMap<Hand, QList<Score> > voices;

    bool operator==(const Song& rhs) const {
        return false;
    }
};

#endif // SONG_H
