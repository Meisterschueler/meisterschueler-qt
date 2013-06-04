#include "guidoview.h"
#include "ui_guidoview.h"

#include <QGraphicsScene>

#include "QGuidoGraphicsItem.h"

#include "song.h"
#include "songservice.h"

GuidoView::GuidoView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GuidoView)
{
    ui->setupUi(this);

    QGraphicsView *gv = ui->graphicsView;

    graphicsScene = new QGraphicsScene(gv);
    gv->setScene(graphicsScene);
    gv->setSceneRect(gv->rect());
    gv->setRenderHint( QPainter::Antialiasing );
    gv->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    gv->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    gv->setAlignment(Qt::AlignTop);

    guidoGraphicsItem = new QGuidoGraphicsItem();
    guidoGraphicsItem->setGMNCode("[\\title<\"Inventio No. 1\"> \\composer<\"J. S. Bach\"> \\meter<\"4/4\"> " + QString("c d e f g").repeated(200) + "]");
    graphicsScene->addItem(guidoGraphicsItem);

    songs = SongService::getSongsFromDirectory("/home/fritz/meisterschueler-misc/scores");
    for (Song song : songs) {
        ui->comboBox->addItem(song.name);
    }
}

GuidoView::~GuidoView()
{
    delete ui;
}

void GuidoView::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GuidoView::resizeEvent(QResizeEvent *e) {
    GuidoDrawBoundingBoxes(0xFF);

    QGraphicsView *gv = ui->graphicsView;

    double ratio = (double)gv->width()/gv->height();
    qDebug("width:%i height:%i ratio: %f", gv->width(), gv->height(), ratio);

    GuidoPageFormat guidoPageFormat;
    GuidoGetDefaultPageFormat(&guidoPageFormat);
    guidoPageFormat.width = GuidoCM2Unit(29.7);
    guidoPageFormat.height = GuidoCM2Unit(29.7/ratio);
    guidoGraphicsItem->setGuidoPageFormat(guidoPageFormat);

    gv->fitInView(guidoGraphicsItem, Qt::KeepAspectRatio);

    // WHY???
    guidoGraphicsItem->setScale(10.0);
}

void GuidoView::on_comboBox_currentIndexChanged(int index)
{
    Song song = songs.at(index);
    guidoGraphicsItem->setGMNCode(song.gmn);
}
