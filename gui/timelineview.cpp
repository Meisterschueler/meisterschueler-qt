#include "timelineview.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPointF>
#include <QPropertyAnimation>

#include <QDebug>

TimelineView::TimelineView(QGraphicsView *parent) : QGraphicsView(parent) {
    graphicsScene = new QGraphicsScene(this);
    setScene(graphicsScene);
    setSceneRect(this->rect());
    setRenderHint( QPainter::Antialiasing );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    originItem = new MyItem();
    originItem->setPos(0, 0);
    originItem->pen().setWidth( 10 );
    originItem->setBrush(QBrush(QColor(255,0,0)));

    graphicsScene->addItem(originItem);

    animation = new QPropertyAnimation(originItem, "pos");
    animation->setDuration(10000000);
    animation->setStartValue(QPointF(0, 0));
    animation->setEndValue(QPointF(-1000000, 0));

    animation->start();
}

TimelineView::~TimelineView()
{
}

void TimelineView::showNoteOnEvent(NoteOnEvent noteOnEvent) {
    double x = this->width() - 30.0;
    double y = (1.0 - noteOnEvent.getNote()/128.0) * this->height();
    QColor color = calcColor(128.0-noteOnEvent.getVelocity(), 50.0, 100.0);

    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(x-originItem->pos().x()-10, y-10, 20, 20, originItem);
    ellipseItem->setPen(QPen(Qt::red));
    ellipseItem->setBrush(QBrush(color));
}

void TimelineView::showNoteOffEvent(NoteOffEvent noteOffEvent) {
    // Do nothing...
}

void TimelineView::showMidiPairCluster(MidiPairCluster midiPairCluster) {
    int idx = midiPairClusters.indexOf(midiPairCluster);
    if (idx == -1) {
        midiPairClusters.append(midiPairCluster);
    } else {
        midiPairClusters.replace(idx, midiPairCluster);
    }

    // Unfinished...
}

QColor TimelineView::calcColor(double value, double vMin, double vMax, double cMin, double cMax) {
    QColor color;

    double normValue = (qBound(vMin, value, vMax) - vMin) / (vMax-vMin);
    normValue = normValue*(cMax-cMin)+cMin;

    if (normValue >= 0.0 && normValue < 1.0)
        color = QColor(255*normValue, 0, 0);
    else if (normValue >= 1.0 && normValue < 2.0)
        color = QColor(255, 255*(normValue-1.0), 0);
    else if (normValue >= 2.0 && normValue < 3.0)
        color = QColor(255*(3.0-normValue), 255, 0);
    else if (normValue >= 3.0 && normValue < 4.0)
        color = QColor(0, 255, 255*(normValue-3.0));
    else if (normValue >= 4.0 && normValue < 5.0)
        color = QColor(0, 255*(5.0-normValue), 255);
    else if (normValue >= 5.0 && normValue < 6.0)
        color = QColor(0, 0, 255*(6.0-normValue));

    return color;
}

void TimelineView::resizeEvent(QResizeEvent *event) {
    setSceneRect(this->rect());
}

void TimelineView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);

    QPointF pointF = mapFromParent(event->pos());

    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(pointF.x()-originItem->pos().x()-10, pointF.y()-10, 20, 20, originItem);
    ellipseItem->setPen(QPen(Qt::red));
    ellipseItem->setBrush(QBrush(QColor(255,0,0)));
}
