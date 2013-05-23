#include "songservice.h"

#include "matchingitem.h"
#include "hanonsongfactory.h"
#include "scoreservice.h"

SongService::SongService()
{
}

QList<MatchingItem> SongService::getMatchingItems() {
    QList<MatchingItem> result;

    HanonSongFactory hanonSongFactory;

    for (Song song : hanonSongFactory.getSongs()) {
        QList<Hand> haende = song.voices.keys();
        QList<Score> scores = song.voices.value(LEFT);
        scores.at(0);
        haende.at(0);

        QByteArray scorePitchSequence = ScoreService::scoresToPitchSequence(song.voices.value(LEFT));
        QByteArray scoreIntervallSequence = ScoreService::scoresToIntervalSequence(song.voices.value(LEFT));
        MatchingItem item(song, scorePitchSequence, scoreIntervallSequence);

        result.append(item);
    }

    return result;
}
