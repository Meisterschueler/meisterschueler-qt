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

    QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(x-originItem->pos().x()-10, y-10, 20, 20, originItem);
    ellipseItem->setPen(QPen(Qt::red));
    ellipseItem->setBrush(QBrush(QColor(32,32,32)));

    MidiPair midiPair(noteOnEvent);
    midiPairItemMap.insert(midiPair, ellipseItem);

    updateLegatoColor();
}

void TimelineView::showNoteOffEvent(NoteOffEvent noteOffEvent) {
    for (MidiPair midiPair : midiPairItemMap.keys()) {
        if (midiPair.noteOn.getNote() == noteOffEvent.getNote() && midiPair.noteOff == emptyNoteOffEvent) {
            QGraphicsEllipseItem *myItem = midiPairItemMap.value(midiPair);

            midiPairItemMap.remove(midiPair);

            midiPair.noteOff = noteOffEvent;
            midiPairItemMap.insert(midiPair, myItem);
            break;
        }
    }

    updateLegatoColor();
}

void TimelineView::updateLegatoColor() {
    for (int i=1; i<midiPairItemMap.keys().count(); i++) {
        MidiPair midiPairPre = midiPairItemMap.keys().at(i-1);
        MidiPair midiPair = midiPairItemMap.keys().at(i);
        QGraphicsEllipseItem *myItem = midiPairItemMap.value(midiPair);

        if (midiPairPre.noteOff != emptyNoteOffEvent && midiPair.noteOn != emptyNoteOnEvent) {
            double delta = midiPair.noteOn.getTime() - midiPairPre.noteOff.getTime();
            qDebug() << delta;
            if (delta > 100) { // staccato
                myItem->setBrush(QBrush(QColor(255,0,0)));
            } else if (delta < -100) { // legato
                myItem->setBrush(QBrush(QColor(0,0,255)));
            }
        }
    }
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
