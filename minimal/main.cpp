#include <QApplication>

#include <QGraphicsColorizeEffect>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "GUIDOScoreMap.h"

#include "QGuidoGraphicsItem.h"

#include "guidoservice.h"
#include "score.h"

class MyGraphicsItem : public QGraphicsRectItem
{
public:
    explicit MyGraphicsItem(const QRectF &rect, QGraphicsItem *parent = 0);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

MyGraphicsItem::MyGraphicsItem(const QRectF &rect, QGraphicsItem *parent) : QGraphicsRectItem(rect, parent)
{
}

void MyGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Score score = qvariant_cast<Score>(this->data(0));
    qDebug("MousePressEvent. Pitch:%i Voice:%i", score.pitch, score.voice);
}

class MyMapCollector : public MapCollector
{
public:
    virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos );
    QList<MapElement> mapElements;
};

void MyMapCollector::Graph2TimeMap(const FloatRect &box, const TimeSegment &dates, const GuidoElementInfos &infos) {

    RectInfos rectInfos(dates, infos);
    MapElement mapElement = std::pair<FloatRect, RectInfos>(box, rectInfos);

    mapElements.append(mapElement);
}

class MyView : public QGraphicsView
{
public:
    explicit MyView(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QGraphicsScene *graphicsScene;
    QGuidoGraphicsItem *guidoGraphicsItem;
};

MyView::MyView(QWidget *parent) : QGraphicsView(parent) {
    graphicsScene = new QGraphicsScene(this);
    setScene(graphicsScene);
    setSceneRect(this->rect());
    setRenderHint( QPainter::Antialiasing );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void MyView::resizeEvent(QResizeEvent *event) {
    QString gmn = "{"
            "[\\composer<\"J. S. Bach\">\\title<\"Inventio 1\">\\key<\"0\">\\meter<\"4/4\">_/16 c1/16 d e f d e c g/8 c2 b1 c2 d/16 g1 a b c2 a1 b g d2/8 g f g e/16 a g f e g f a g f e d c e d f e d c b1 a c2 b1 d2 c b1 a g f# a g b a/8 d c2/8. d/16 b1 a g f# e g f# a g b a c2 b1 d2 c e d b1/32 c2 d/16 g b1/8 a/16 g g/8 _/8 _/4 _/16 g/16 a b c2 a1 b g f#/8 _/8 _/4 _/16 a/16 b c2 d b1 c2 a1 b/8 _/8 _/4 _/16 d2/16 c b1 a c2 b1 d2 c/8 _/8 _/4 _/16 e/16 d c b1 d2 c# e d/8 c# d e f a1 b c#2 d f#1 g# a b c2 d*5/16 e1/16 f# g# a f# g# e e2 d c e d c b1 d2 c a g# b a e f d a&1 f2 e d c/8 b1/16 a a a2 g f e g f a g*9/16 e/16 f g a f g e f*9/16 g/16 f e d f e g f*9/16 d/16 e f g e f d e*9/16 c/16 d e f d e c d e f g a f g e f g a b c3 a2 b g c3/8 g2 e d/16 c c b&1 a g f a g b& a b c2 e1 d c2 f1 b {e/1,g,c2} ],"
            "[\\key<\"0\">\\meter<\"4/4\">_/2 _/16 c0/16 d e f d e c g/8 g-1 _/4 _/16 g0/16 a b c1 a0 b g c1/8 b0 c1 d e g0 a b c1 e0 f# g a b c1*5/16 d0/16 e f# g e f# d g/8 b-1 c0 d e f# g e b-1/8. c0/16 d/8 d-1 _/16 g/16 a b c0 a-1 b g d0/8 g f# g a/16 d e f# g e f# d a/8 d1 c d g0/16 g1 f e d f e g f/8 e f d e/16 a g f e g f a g/8 f g e f/16 b& a g f a g b& a g f e d f e g f e d c b0 d1 c e d c b0 a g# b a c1 b0/8 e d1/8. e/16 c b0 a g f# a g# b a c1 b0 d1 c e d f e/8 a0 e1 e0 a a-1 _/4 _/16 e1/16 d c b0 d1 c# e d*9/16 a0/16 b c1 d b0 c1 a0 b*9/16 d1/16 c b0 a c1 b0 d1 c*9/16 g0/16 a b& c1 a0 b& g a/8 b& a g f d1 c b&0 a f1 e d e/16 d0 e f g e f d e/8 c d e f/16 d e f g/8 g-1 {c/1,c0} ]"
            "}";

    QList<Score> scores = GuidoService::gmnToScores(gmn);

    graphicsScene->clear();

    guidoGraphicsItem = new QGuidoGraphicsItem();
    guidoGraphicsItem->setGMNCode(gmn);
    graphicsScene->addItem(guidoGraphicsItem);

    double ratio = (double)width()/height();

    GuidoPageFormat guidoPageFormat;
    GuidoGetDefaultPageFormat(&guidoPageFormat);
    guidoPageFormat.width = GuidoCM2Unit(29.7);
    guidoPageFormat.height = GuidoCM2Unit(29.7/ratio);
    guidoPageFormat.marginright = 0;
    guidoGraphicsItem->setGuidoPageFormat(guidoPageFormat);

    MyMapCollector myMapCollector;
    GuidoErrCode guidoErrCode = GuidoGetMap(guidoGraphicsItem->getGRHandler(), 1, 317, 317/ratio, kGuidoEvent, myMapCollector);
    for (MapElement mapElement : myMapCollector.mapElements) {
        if (mapElement.second.infos().type != kNote)
            continue;

        FloatRect floatRect = mapElement.first;
        QRectF rect;
        rect.setCoords(floatRect.left, floatRect.top, floatRect.right, floatRect.bottom);

        QPen pen(QBrush(QColor(255,0,0)), 3);
        MyGraphicsItem *item = new MyGraphicsItem(rect);
        item->setPen(pen);

        Score score = GuidoService::getScore(scores, mapElement);
        QVariant v;
        v.setValue<Score>(score);
        item->setData(0, v);

        graphicsScene->addItem(item);
    }

    fitInView(guidoGraphicsItem, Qt::KeepAspectRatio);
}

int main(int argc, char *argv[])
{
    QGuidoPainter::startGuidoEngine();

    QApplication a(argc, argv);
    MyView w;
    w.show();

    int result = a.exec();

    QGuidoPainter::stopGuidoEngine();
    
    return result;
}
