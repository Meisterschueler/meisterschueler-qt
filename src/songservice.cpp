#include "songservice.h"

#include <QDebug>
#include <QDir>

#include "matchingitem.h"
#include "hanonsongfactory.h"
#include "scoreservice.h"

SongService::SongService()
{
}

QList<Song> SongService::getSongsBuiltIn() {
    HanonSongFactory hanonSongFactory;
    return hanonSongFactory.getSongs();
}

QList<Song> SongService::getSongsFromDirectory(const QString& directory) {
    QList<Song> result;

    QStringList gmnFileNames = getGmnFileNames(directory);

    for (QString gmnFileName : gmnFileNames) {
        QFile gmnFile(gmnFileName);

        if (!gmnFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open: " << gmnFileName;
            continue;
        }

        QTextStream ts(&gmnFile);
        QString gmn = ts.readAll();
        gmnFile.close();

        Song song;
        song.gmn = gmn;
        song.name = gmnFile.fileName();

        result.append(song);
    }

    return result;
}

QList<MatchingItem> SongService::createMatchingItems(const QList<Song>& songs) {
    QList<MatchingItem> result;

    for (Song song : songs) {
        QList<Hand> haende = song.voices.keys();
        QList<Score> scores = song.voices.value(LEFT);

        if (scores.size() == 0) {
            continue;
        }

        QByteArray scorePitchSequence = ScoreService::scoresToPitchSequence(song.voices.value(LEFT));
        QByteArray scoreIntervallSequence = ScoreService::scoresToIntervalSequence(song.voices.value(LEFT));
        MatchingItem item(song, scorePitchSequence, scoreIntervallSequence);

        result.append(item);
    }

    return result;
}

int SongService::countSpecialInterval(const Song &song, const int &interval, const Finger &fingerFrom, const Finger &fingerTo) {
    int count = 0;
    QList<Score> scores = song.voices.value(Hand::LEFT);
    for (int i=1; i<scores.count(); i++) {
        Score from = scores.at(i-1);
        Score to = scores.at(i);
        if (interval == to.pitch-from.pitch && from.finger == fingerFrom && to.finger == fingerTo) {
            count++;
        }
    }
    return count;
}

QStringList SongService::getGmnFileNames(const QString& directory) {
    QStringList result;

    QDir dir(directory);

    dir.setNameFilters(QStringList("*.gmn"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    //qDebug() << "Scanning: " << dir.path();

    QStringList fileList = dir.entryList();
    for (int i=0; i<fileList.count(); i++) {
        QString fileName = QString("%1/%2").arg(dir.absolutePath()).arg(fileList.at(i));
        //qDebug() << "Found file: " << fileName;
        result.append(fileName);
    }

    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList dirList = dir.entryList();
    for (int i=0; i<dirList.size(); ++i) {
        QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i));
        result.append(getGmnFileNames(newPath));
    }

    return result;
}
