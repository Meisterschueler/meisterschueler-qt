#include "graphicsscoreitem.h"

#include <QPen>

#include "score.h"

GraphicsScoreItem::GraphicsScoreItem(const QRectF &rect, QGraphicsItem *parent) : QGraphicsRectItem(rect, parent)
{
    QPen pen(QBrush(QColor(255,0,0)), 3);
    setPen(pen);
}

void GraphicsScoreItem::setScore(const Score &score) {
    QVariant v;
    v.setValue<Score>(score);
    setData(0, v);
}

void GraphicsScoreItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Score score = qvariant_cast<Score>(this->data(0));
    qDebug("MousePressEvent. Pitch:%i Voice:%i", score.pitch, score.voice);
}
