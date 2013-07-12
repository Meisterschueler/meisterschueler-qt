#ifndef GRAPHICSSCOREITEM_H
#define GRAPHICSSCOREITEM_H

#include <QObject>
#include <QGraphicsRectItem>

#include "events.h"

class Score;

class GraphicsScoreItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit GraphicsScoreItem(const QRectF &rect, QGraphicsItem *parent = 0);
    void setScore(const Score& score);
    Score getScore() const;
    
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);

public slots:
    
};


#endif // GRAPHICSSCOREITEM_H
