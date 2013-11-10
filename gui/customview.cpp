#include "customview.h"

#include "statisticsservice.h"

CustomView::CustomView(QWidget *parent) :
    QCustomPlot(parent)
{
    this->addGraph();
    this->graph(0)->setPen(QColor(50, 50, 50, 255));
    this->graph(0)->setLineStyle(QCPGraph::lsNone);
    this->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    this->xAxis->setLabel("speed");
    this->xAxis->setRange(40, 208);

    this->yAxis->setLabel("velocity");
    this->yAxis->setRange(20, 80);
}

void CustomView::showMidiPairs(QList<MidiPair> midiPairs) {
    StatisticCluster statisticCluster = StatisticsService::getStatisticCluster(midiPairs);

    QVector<double> xData;
    QVector<double> yData;

    for(double value : statisticCluster.speed.values) {
        xData.append(value/4.0);
    }
    yData = statisticCluster.velocity.values;

    this->graph(0)->setData(xData, yData);
    this->replot();
}
