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

    QPoint toSoundCoords(const QPoint& pos);
    bool makeSound(const QPoint& soundCoords);
    void makeBubble(const QPoint& pos);

    QPixmap getBackgroundPixmap();

    QPoint soundCoords;

public slots:
    void showNoteOnEvent(NoteOnEvent event);

signals:
//    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // BUBBLEVIEW_H
