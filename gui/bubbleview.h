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
    QPoint soundCoords;

    QList<MidiPair> oldMidiPairs;
    QMap<MidiPair, BubbleGraphicsItem*> midiPairBubbleMap;

    BubbleGraphicsItem *makeBubble();
    QPixmap getBackgroundPixmap();

    void attachDisappearingAnimation(BubbleGraphicsItem *bubble, const QPoint& pos);

    QPoint toSoundCoords(const QPoint& pos);
    bool makeSound(const QPoint& soundCoords);

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
