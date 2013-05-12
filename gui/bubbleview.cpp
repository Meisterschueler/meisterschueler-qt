#include "bubbleview.h"

#include <QPropertyAnimation>

BubbleView::BubbleView(QGraphicsView *parent) :
    QGraphicsView(parent)
{
    graphicsScene = new QGraphicsScene(this);
    setScene(graphicsScene);
    setSceneRect(this->rect());
    setRenderHint( QPainter::Antialiasing );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
}

BubbleView::~BubbleView()
{
}

void BubbleView::makeBubble( const QPoint& pos ) {
    BubbleGraphicsItem *bubble = new BubbleGraphicsItem();
    bubble->pen().setWidth( 10 );
    bubble->setFlag( QGraphicsItem::ItemIgnoresTransformations );

    graphicsScene->addItem( bubble );

    QPropertyAnimation *animRect = new QPropertyAnimation(bubble, "rect", this);
    animRect->setStartValue( QRectF(pos.x()-10, pos.y()-10, 20, 20) );
    animRect->setEndValue( QRectF(pos.x()-80, pos.y()-80, 160, 160) );
    animRect->setDuration(2000);
    animRect->start();

    QPropertyAnimation *animOpacity = new QPropertyAnimation(bubble, "opacity", this);
    animOpacity->setEasingCurve( QEasingCurve::InQuad );
    animOpacity->setStartValue(1.0);
    animOpacity->setEndValue(0.0);
    animOpacity->setDuration(2000);
    animOpacity->start();

    QPropertyAnimation *animPenColor = new QPropertyAnimation(bubble, "penColor", this);
    animPenColor->setStartValue(0.0);
    animPenColor->setEndValue(1.0);
    animPenColor->setDuration(2000);
    animPenColor->start();
}

void BubbleView::resizeEvent(QResizeEvent *event) {
    //fitInView(backgroundItem);
}

void BubbleView::mousePressEvent( QMouseEvent *event ) {
    QGraphicsView::mousePressEvent(event);
    QPoint pos = event->pos();
    makeBubble(pos);
}

void BubbleView::mouseMoveEvent( QMouseEvent *event ) {
    QGraphicsView::mouseMoveEvent(event);
    QPoint pos = event->pos();
    makeBubble(pos);
}
