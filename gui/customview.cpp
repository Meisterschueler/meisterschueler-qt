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


    ellipse = new QCPItemEllipse(this);
    ellipse->setPen(QColor(255, 0, 0, 255));
    ellipse->setBrush(QBrush(QColor(255, 0, 0, 32)));
    this->addItem(ellipse);

    ellipseLM = new QCPItemEllipse(this);
    ellipseLM->setPen(QColor(255, 0, 0, 255));
    ellipseLM->setBrush(QBrush(QColor(0, 255, 0, 64)));
    this->addItem(ellipseLM);

    xAxisType = SPEED;
    yAxisType = VELOCITY;
    colorization = NONE;

    updateDiagram();
}

void CustomView::showMidiPairs(QList<MidiPair> midiPairs) {
    StatisticCluster statisticCluster = StatisticsService::getStatisticCluster(midiPairs, Frac_1_16);

    StatisticItem x;
    StatisticItem xLM;
    StatisticItem y;
    StatisticItem yLM;

    switch (xAxisType) {
    case SPEED:
        x = statisticCluster.speed;
        xLM = statisticCluster.speedLastMeasure;
        break;
    case VELOCITY:
        x = statisticCluster.velocity;
        xLM = statisticCluster.velocityLastMeasure;
        break;
    case OVERLAP:
        x = statisticCluster.overlap;
        xLM = statisticCluster.overlapLastMeasure;
        break;
    }

    switch (yAxisType) {
    case SPEED:
        y = statisticCluster.speed;
        yLM = statisticCluster.speedLastMeasure;
        break;
    case VELOCITY:
        y = statisticCluster.velocity;
        yLM = statisticCluster.velocityLastMeasure;
        break;
    case OVERLAP:
        y = statisticCluster.overlap;
        yLM = statisticCluster.overlapLastMeasure;
        break;
    }

    QVector<double> xData;
    QVector<double> yData;

    xData = x.values;
    yData = y.values;

    this->graph(0)->setData(xData, yData);

    ellipse->topLeft->setCoords(x.mean-x.standarddeviation,
                                y.mean+y.standarddeviation);

    ellipse->bottomRight->setCoords(x.mean+x.standarddeviation,
                                y.mean-y.standarddeviation);

    ellipseLM->topLeft->setCoords(xLM.mean-xLM.standarddeviation,
                                  yLM.mean+yLM.standarddeviation);

    ellipseLM->bottomRight->setCoords(xLM.mean+xLM.standarddeviation,
                                  yLM.mean-yLM.standarddeviation);


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
    case OVERLAP:
        this->xAxis->setLabel("overlap");
        this->xAxis->setRange(-250, 250);
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
    case OVERLAP:
        this->xAxis->setLabel("overlap");
        this->xAxis->setRange(-50, 50);
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
    QAction *xOverlap = xMenu.addAction(tr("Overlap"));

    QMenu yMenu;
    yMenu.setTitle(tr("y-Axis"));
    QAction *ySpeed = yMenu.addAction(tr("Speed"));
    QAction *yVelocity = yMenu.addAction(tr("Velocity"));
    QAction *yOverlap = yMenu.addAction(tr("Overlap"));

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
        } else if (selectedItem == xOverlap) {
            xAxisType = OVERLAP;
        } else if (selectedItem == ySpeed) {
            yAxisType = SPEED;
        } else if (selectedItem == yVelocity) {
            yAxisType = VELOCITY;
        } else if (selectedItem == yOverlap) {
            yAxisType = OVERLAP;
        } else if (selectedItem == noneColorization) {
            colorization = NONE;
        } else if (selectedItem == fingerColorization) {
            colorization = FINGER;
        }
    }

    updateDiagram();
}
