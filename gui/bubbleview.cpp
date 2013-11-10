#include "bubbleview.h"

#include <QPropertyAnimation>

BubbleView::BubbleView(QGraphicsView *parent) :
    QGraphicsView(parent), soundCoords(-1, -1)
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

BubbleGraphicsItem *BubbleView::makeBubble() {
    BubbleGraphicsItem *bubble = new BubbleGraphicsItem();
    bubble->pen().setWidth( 10 );
    bubble->setFlag( QGraphicsItem::ItemIgnoresTransformations );

    graphicsScene->addItem( bubble );

    return bubble;
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

void BubbleView::attachDisappearingAnimation(BubbleGraphicsItem *bubble, const QPoint& pos) {
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

QPoint BubbleView::toSoundCoords(const QPoint& pos) {
    QPointF itemCoords = mapToScene(pos);
    return QPoint(itemCoords.x(), 127-itemCoords.y());
}

bool BubbleView::makeSound(const QPoint& soundCoords) {
    bool coordinatesChanged = this->soundCoords != soundCoords;
    if (coordinatesChanged) {

        if (this->soundCoords != QPoint(-1, -1)) {
            NoteOffEvent noteOffEvent(0, 0, this->soundCoords.x(), this->soundCoords.y());
            emit gotNoteOffEvent(noteOffEvent);
        }

        this->soundCoords = soundCoords;

        if (this->soundCoords != QPoint(-1, -1)) {
            NoteOnEvent noteOnEvent(0, 0, this->soundCoords.x(), this->soundCoords.y());
            emit gotNoteOnEvent(noteOnEvent);
        }
    }

    return coordinatesChanged;
}

void BubbleView::reset() {
    oldMidiPairs.clear();
    for (BubbleGraphicsItem *bubble : midiPairBubbleMap.values()) {
        graphicsScene->removeItem(bubble);
    }
    midiPairBubbleMap.clear();
}

void BubbleView::showMidiPairs(QList<MidiPair> midiPairs) {
    for (int i = 0; i < midiPairs.count(); ++i) {
        MidiPair midiPair = midiPairs.at(i);
        if (i >= oldMidiPairs.count()) {
            // neues Bubble
            BubbleGraphicsItem *bubble = makeBubble();
            midiPairBubbleMap.insert(midiPair, bubble);

            NoteOnEvent noteOn = midiPair.noteOn;
            QPoint pos = mapFromScene(noteOn.getNote(), 127-noteOn.getVelocity());
            bubble->setRect(QRectF(pos.x()-10, pos.y()-10, 20, 20));
            bubble->setPenColor(0.0);

            if (midiPair.noteOn != emptyNoteOnEvent && midiPair.noteOff != emptyNoteOffEvent) {
                attachDisappearingAnimation(bubble, pos);
            }

        } else if (midiPairs.at(i) != oldMidiPairs.at(i)) {
            // geändertes Bubble
            MidiPair oldMidiPair = oldMidiPairs.at(i);
            BubbleGraphicsItem *bubble = midiPairBubbleMap.value(oldMidiPair);
            midiPairBubbleMap.insert(midiPair, bubble);

            if (midiPair.noteOn != emptyNoteOnEvent && midiPair.noteOff != emptyNoteOffEvent) {
                NoteOnEvent noteOn = midiPair.noteOn;
                QPoint pos = mapFromScene(noteOn.getNote(), 127-noteOn.getVelocity());
                attachDisappearingAnimation(bubble, pos);
            }
        }
    }

    oldMidiPairs = midiPairs;
}

void BubbleView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    fitInView(backgroundItem);
}

void BubbleView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);

    QPoint soundCoords = toSoundCoords(event->pos());
    if (makeSound(soundCoords)) {
        BubbleGraphicsItem *bubble = makeBubble();
        attachDisappearingAnimation(bubble, event->pos());
    }
}

void BubbleView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);

    QPoint soundCoords = toSoundCoords(event->pos());
    if (makeSound(soundCoords)) {
        BubbleGraphicsItem *bubble = makeBubble();
        attachDisappearingAnimation(bubble, event->pos());
    }
}

void BubbleView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    makeSound(QPoint(-1, -1));
}
