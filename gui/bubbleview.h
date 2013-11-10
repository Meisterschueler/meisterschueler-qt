#ifndef BUBBLEVIEW_H
#define BUBBLEVIEW_H

#include <QGraphicsView>
#include <QList>
#include <QMouseEvent>

#include "events.h"
#include "bubblegraphicsitem.h"

class BubbleView : public QGraphicsView
{
    Q_OBJECT
    
public:
    explicit BubbleView(QGraphicsView *parent = 0);
    ~BubbleView();
    
private:
    QGraphicsScene *graphicsScene;
    QGraphicsPixmapItem *backgroundItem;

    QPoint toSoundCoords(const QPoint& pos);
    bool makeSound(const QPoint& soundCoords);
    BubbleGraphicsItem *makeBubble();
    void attachDisappearingAnimation(BubbleGraphicsItem *bubble, const QPoint& pos);

    QPixmap getBackgroundPixmap();

    QPoint soundCoords;

    QList<MidiPair> oldMidiPairs;
    QMap<MidiPair, BubbleGraphicsItem*> midiPairBubbleMap;

public slots:
    void reset();
    void showMidiPairs(QList<MidiPair> midiPairs);

signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // BUBBLEVIEW_H
