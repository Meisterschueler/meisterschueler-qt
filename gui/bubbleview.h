#ifndef BUBBLEVIEW_H
#define BUBBLEVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

#include "bubblegraphicsitem.h"

class BubbleView : public QGraphicsView
{
    Q_OBJECT
    
public:
    explicit BubbleView(QGraphicsView *parent = 0);
    ~BubbleView();
    
private:
    QGraphicsScene *graphicsScene;
    QMap<int, BubbleGraphicsItem> bubbles;

    void makeBubble( const QPoint& pos );

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // BUBBLEVIEW_H
