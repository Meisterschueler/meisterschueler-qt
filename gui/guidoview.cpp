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

    guidoGraphicsItem = new QGuidoGraphicsItem();
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
    QWidget::resizeEvent(e);
    GuidoDrawBoundingBoxes(0xFF);

    double ratio = ui->graphicsView->width()/ui->graphicsView->height();
    double dinA4_width = 29.7;

    GuidoPageFormat pageFormat;
    GuidoGetDefaultPageFormat(&pageFormat);
    pageFormat.width = GuidoCM2Unit(dinA4_width);
    pageFormat.height = GuidoCM2Unit(dinA4_width/ratio);
    guidoGraphicsItem->setGuidoPageFormat(pageFormat);

    QRectF gb = guidoGraphicsItem->boundingRect();
    qDebug("Abmessungen gGI: %f x %f", gb.width(), gb.height());
    qDebug("Abmessungen View: %f x %f", ui->graphicsView->width(), ui->graphicsView->height());

    if (gb.width() == 0 || gb.height() == 0) {
        // We have still a QGuidoGraphicsItem bug

        ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        guidoGraphicsItem->setScale(4.0);
    } else {
        ui->graphicsView->fitInView(guidoGraphicsItem);
    }
}

void GuidoView::on_comboBox_currentIndexChanged(int index)
{
    Song song = songs.at(index);
    guidoGraphicsItem->setGMNCode(song.gmn);
}
