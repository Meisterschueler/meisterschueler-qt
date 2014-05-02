#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QGraphicsView>
#include <QList>
#include <QMouseEvent>

#include "events.h"
#include "bubblegraphicsitem.h"

class QPropertyAnimation;

class MyItem : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
};

class TimelineView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit TimelineView(QGraphicsView *parent = 0);
    ~TimelineView();

private:
    QColor calcColor(double value, double vMin = 0.0, double vMax = 1.0, double cMin = 1.0, double cMax = 5.0);

    QGraphicsScene *graphicsScene;
    QPropertyAnimation *animation;
    MyItem *originItem;

    QList<MidiPairCluster> midiPairClusters;
    QMap<MidiPairCluster, QGraphicsEllipseItem*> midiPairItemMap;

public slots:
    void showNoteOnEvent(NoteOnEvent noteOnEvent);
    void showNoteOffEvent(NoteOffEvent noteOffEvent);
    void showMidiPairCluster(MidiPairCluster midiPairCluster);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // TIMELINEVIEW_H
