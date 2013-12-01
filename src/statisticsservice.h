#ifndef STATISTICSSERVICE_H
#define STATISTICSSERVICE_H

#include <QVector>

class MidiPair;
class MidiPairCluster;
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
    StatisticItem overlap;
    StatisticItem acceleration;
    StatisticItem offset;

    StatisticItem speedLastMeasure;
    StatisticItem velocityLastMeasure;
    StatisticItem overlapLastMeasure;
    StatisticItem accelerationLastMeasure;
    StatisticItem offsetLastMeasure;
};

class StatisticsService
{
public:
    StatisticsService();

    static StatisticItem getStatisticItem(const QVector<double>& values);
    static StatisticCluster getStatisticCluster(const QList<MidiPairCluster>& mpcs, const Fraction& fraction = Frac_1_4);
    static StatisticCluster getStatisticCluster(const QList<Score>& scores);
};

#endif // STATISTICSSERVICE_H
