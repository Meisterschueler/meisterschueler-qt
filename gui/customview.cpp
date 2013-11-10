#include "customview.h"

#include <QAction>

#include "statisticsservice.h"

CustomView::CustomView(QWidget *parent) :
    QCustomPlot(parent)
{    
    this->addGraph();
    this->graph(0)->setPen(QColor(50, 50, 50, 255));
    this->graph(0)->setLineStyle(QCPGraph::lsNone);
    this->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    xAxisType = SPEED;
    yAxisType = VELOCITY;
    colorization = NONE;

    updateDiagram();
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

void CustomView::updateDiagram() {
    switch (xAxisType) {
    case SPEED:
        this->xAxis->setLabel("speed");
        this->xAxis->setRange(40, 208);
        break;
    case VELOCITY:
        this->xAxis->setLabel("velocity");
        this->xAxis->setRange(20, 80);
        break;
    }

    switch (yAxisType) {
    case SPEED:
        this->yAxis->setLabel("speed");
        this->yAxis->setRange(40, 208);
        break;
    case VELOCITY:
        this->yAxis->setLabel("velocity");
        this->yAxis->setRange(20, 80);
        break;
    }

    switch (colorization) {
    case NONE:
        // bla
        break;
    case FINGER:
        // bla
        break;
    }

    this->replot();
}

void CustomView::contextMenuEvent(QContextMenuEvent *event) {
    QMenu xMenu;
    xMenu.setTitle(tr("x-Axis"));
    QAction *xSpeed = xMenu.addAction(tr("Speed"));
    QAction *xVelocity = xMenu.addAction(tr("Velocity"));

    QMenu yMenu;
    yMenu.setTitle(tr("y-Axis"));
    QAction *ySpeed = yMenu.addAction(tr("Speed"));
    QAction *yVelocity = yMenu.addAction(tr("Velocity"));

    QMenu colorizationMenu;
    colorizationMenu.setTitle(tr("Colorization"));
    QAction *noneColorization = colorizationMenu.addAction(tr("None"));
    QAction *fingerColorization = colorizationMenu.addAction(tr("Finger"));

    QMenu contextMenu;
    contextMenu.addMenu(&xMenu);
    contextMenu.addMenu(&yMenu);
    contextMenu.addMenu(&colorizationMenu);

    QAction* selectedItem = contextMenu.exec(event->pos());
    if (selectedItem) {
        if (selectedItem == xSpeed) {
            xAxisType = SPEED;
        } else if (selectedItem == xVelocity) {
            xAxisType = VELOCITY;
        } else if (selectedItem == ySpeed) {
            yAxisType = SPEED;
        } else if (selectedItem == yVelocity) {
            yAxisType = VELOCITY;
        } else if (selectedItem == noneColorization) {
            colorization = NONE;
        } else if (selectedItem == fingerColorization) {
            colorization = FINGER;
        }
    }

    updateDiagram();
}
