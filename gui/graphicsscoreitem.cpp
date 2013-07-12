#include "graphicsscoreitem.h"

#include <QPen>

#include "score.h"

GraphicsScoreItem::GraphicsScoreItem(const QRectF &rect, QGraphicsItem *parent) : QGraphicsRectItem(rect, parent)
{
    QPen pen(QBrush(QColor(255,0,0)), 1);
    setPen(pen);
}

void GraphicsScoreItem::setScore(const Score &score) {
    QVariant v;
    v.setValue<Score>(score);
    setData(0, v);
}

Score GraphicsScoreItem::getScore() const {
    return qvariant_cast<Score>(this->data(0));
}

void GraphicsScoreItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event)
    Score score = getScore();
    emit gotNoteOnEvent(NoteOnEvent(0, 0, score.pitch, 64));
}

void GraphicsScoreItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event)
    Score score = getScore();
    emit gotNoteOffEvent(NoteOffEvent(0, 0, score.pitch, 0));
}
