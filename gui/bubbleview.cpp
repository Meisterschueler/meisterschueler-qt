#include "bubbleview.h"

#include <QPropertyAnimation>

BubbleView::BubbleView(QGraphicsView *parent) :
    QGraphicsView(parent), pitch(-1), keystroke(-1)
{
    graphicsScene = new QGraphicsScene(this);
    setScene(graphicsScene);
    setSceneRect(this->rect());
    setRenderHint( QPainter::Antialiasing );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    backgroundItem = graphicsScene->addPixmap( getBackgroundPixmap() );
}

BubbleView::~BubbleView()
{
}

void BubbleView::makeBubble( const QPoint& pos ) {
    QPointF itemCoords = mapToScene(pos);
    int pitch_temp = static_cast<int>(itemCoords.x());
    int keystroke_temp = 127-static_cast<int>(itemCoords.y());
    bool coordinatesChanged = (pitch != pitch_temp || keystroke != keystroke_temp);
    if (coordinatesChanged) {
        pitch = pitch_temp;
        keystroke = keystroke_temp;
        emit gotNoteOnEvent(pitch, keystroke);
    } else {
        return;
    }

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

void BubbleView::showNoteOnEvent(NoteOnEvent event) {
    QPoint itemCoords(event.getNote(), 127-event.getVelocity());
    QPoint pos = mapFromScene(itemCoords);
    makeBubble(pos);
}

void BubbleView::dummySlot(int note, int velocity) {
    QPoint itemCoords(note, 127-velocity);
    QPoint pos = mapFromScene(itemCoords);
    makeBubble(pos);
}

QPixmap BubbleView::getBackgroundPixmap() {
    QImage image(128, 128, QImage::Format_ARGB32);

    QPainter painter(&image);
    painter.fillRect(image.rect(), Qt::white);

    QVector<int> blackKeys {1, 3, 6, 8, 10};

    for (int x = 0; x < 128; ++x) {
        for (int y = 0; y < 128; ++y) {
            int gray;
            if (blackKeys.contains(x % 12)) {
                gray = 220;
            } else {
                gray = 250;
            }

            if ((x + y) % 2 == 0) {
                gray += 5;
            }

            image.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }

    return QPixmap::fromImage(image);
}

void BubbleView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    fitInView(backgroundItem);
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

void BubbleView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    pitch = -1;
    keystroke = -1;
}
