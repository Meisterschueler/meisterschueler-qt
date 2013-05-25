#ifndef BUBBLEVIEW_H
#define BUBBLEVIEW_H

#include <QGraphicsView>
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

    QMap<int, BubbleGraphicsItem> bubbles;

    void makeBubble( const QPoint& pos );
    QPixmap getBackgroundPixmap();

    int pitch;
    int keystroke;

public slots:
    void showNoteOnEvent(NoteOnEvent event);
    void dummySlot(int gotNoteOnEvent, int velocity);

signals:
    void gotNoteOnEvent(int pitch, int keystroke);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // BUBBLEVIEW_H
