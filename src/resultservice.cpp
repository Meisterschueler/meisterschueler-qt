#include "resultservice.h"

#include <QDate>
#include <QTime>

#include "midiservice.h"

ResultService::ResultService(QObject *parent) :
    QObject(parent)
{
}

void ResultService::analyseFinishedSong(MatchingItem item) {
    QList<ChannelEvent> channelEvents;

    for (Score score : item.mergedScores) {
        channelEvents.append(*score.midiPair.noteOn);
        channelEvents.append(*score.midiPair.noteOff);
    }

    QString filename = item.song.name + "_" + QDate::currentDate().toString("yyMMdd") + QTime::currentTime().toString("HHmmss");
    MidiService::save(filename, channelEvents);
}
