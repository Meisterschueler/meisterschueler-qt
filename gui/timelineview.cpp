#include "timelineview.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPointF>
#include <QPropertyAnimation>

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

    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(x-originItem->pos().x()-10, y-10, 20, 20, originItem);
    ellipseItem->setPen(QPen(Qt::red));
    ellipseItem->setBrush(QBrush(QColor(255,0,0)));
}

void TimelineView::showNoteOffEvent(NoteOffEvent noteOffEvent) {

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
