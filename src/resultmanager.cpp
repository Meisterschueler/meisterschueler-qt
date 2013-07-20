#include "resultmanager.h"

#include <QDate>
#include <QDebug>
#include <QTime>

#include "midiservice.h"
#include "statisticsservice.h"

ResultManager::ResultManager(QObject *parent) :
    QObject(parent)
{
}

void ResultManager::analyseFinishedSong(MatchingItem item) {

    qDebug() << item.pitchAlignment;

    StatisticCluster statisticCluster = StatisticsService::getStatisticCluster(item.mergedScores);
    StatisticItem speed = statisticCluster.speed;
    qDebug() << "Speed:" << speed.min << speed.mean << speed.max;

    StatisticItem velocity = statisticCluster.velocity;
    qDebug() << "Velocity:" << velocity.min << velocity.mean << velocity.max;
}

void ResultManager::saveFinishedSong(MatchingItem item) {
    QList<ChannelEvent> channelEvents;

    for (Score score : item.mergedScores) {
        channelEvents.append(score.midiPair.noteOn);
        channelEvents.append(score.midiPair.noteOff);
    }

    QString filename = item.song.name + "_" + QDate::currentDate().toString("yyMMdd") + QTime::currentTime().toString("HHmmss") + ".mid";
    MidiService::save(filename, channelEvents);
}
