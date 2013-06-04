#include <QApplication>

#include <QGraphicsView>
#include <QGraphicsScene>

#include "QGuidoGraphicsItem.h"

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
    setAlignment(Qt::AlignTop);

    guidoGraphicsItem = new QGuidoGraphicsItem();
    guidoGraphicsItem->setGMNCode("[\\title<\"Inventio No. 1\"> \\composer<\"J. S. Bach\"> \\meter<\"4/4\"> " + QString("c d e f g").repeated(200) + "]");
    graphicsScene->addItem(guidoGraphicsItem);
}

void MyView::resizeEvent(QResizeEvent *event) {
    double ratio = (double)width()/height();
    qDebug("width:%i height:%i ratio: %f", width(), height(), ratio);

    GuidoPageFormat guidoPageFormat;
    GuidoGetDefaultPageFormat(&guidoPageFormat);
    guidoPageFormat.width = GuidoCM2Unit(29.7);
    guidoPageFormat.height = GuidoCM2Unit(29.7/ratio);
    guidoGraphicsItem->setGuidoPageFormat(guidoPageFormat);

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
