#include "resultmanager.h"

#include <QDate>
#include <QTime>

#include "midiservice.h"

ResultManager::ResultManager(QObject *parent) :
    QObject(parent)
{
}

void ResultManager::analyseFinishedSong(MatchingItem item) {
    QList<ChannelEvent> channelEvents;

    for (Score score : item.mergedScores) {
        channelEvents.append(score.midiPair.noteOn);
        channelEvents.append(score.midiPair.noteOff);
    }

    QString filename = item.song.name + "_" + QDate::currentDate().toString("yyMMdd") + QTime::currentTime().toString("HHmmss");
    MidiService::save(filename, channelEvents);
}
