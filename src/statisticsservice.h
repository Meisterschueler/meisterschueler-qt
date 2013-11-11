#ifndef STATISTICSSERVICE_H
#define STATISTICSSERVICE_H

#include <QVector>

class MidiPair;
class Score;

struct StatisticItem {
    QVector<double> values;
    double min;
    double max;
    double mean;
    double variance;
    double standarddeviation;
    QVector<double> spectrum;
};

struct StatisticCluster {
    StatisticItem speed;
    StatisticItem velocity;
    StatisticItem speedLast16;
    StatisticItem velocityLast16;
};

class StatisticsService
{
public:
    StatisticsService();

    static StatisticItem getStatisticItem(const QVector<double>& values);
    static StatisticCluster getStatisticCluster(const QList<MidiPair>& midiPairs);
    static StatisticCluster getStatisticCluster(const QList<Score>& scores);
};

#endif // STATISTICSSERVICE_H
