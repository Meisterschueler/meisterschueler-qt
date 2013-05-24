#ifndef STATISTICSSERVICE_H
#define STATISTICSSERVICE_H

#include <QVector>

class Score;

struct StatisticItem {
    double min;
    double max;
    double mean;
    double variance;
    QVector<double> spectrum;
};

struct StatisticCluster {
    StatisticItem speed;
    StatisticItem velocity;
};

class StatisticsService
{
public:
    StatisticsService();

    static StatisticItem getStatisticItem(const QVector<double>& values);
    static StatisticCluster getStatisticCluster(const QList<Score>& scores);
};

#endif // STATISTICSSERVICE_H
