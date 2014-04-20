#include "guidoview.h"
#include "ui_guidoview.h"

#include <QGraphicsScene>
#include <QKeyEvent>

#include "QGuidoGraphicsItem.h"

#include "graphicsscoreitem.h"
#include "guidoservice.h"
#include "mymapcollector.h"
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
    gv->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

GuidoView::~GuidoView()
{
    delete ui;
}

void GuidoView::setSongs(const QList<Song>& songs) {
    this->songs = songs;
    ui->comboBox->clear();
    for (Song song : songs) {
        ui->comboBox->addItem(song.name);
    }
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

    QGraphicsView *gv = ui->graphicsView;
    double ratio = (double)gv->width()/gv->height();

    GuidoPageFormat guidoPageFormat;
    GuidoGetDefaultPageFormat(&guidoPageFormat);
    guidoPageFormat.width = GuidoCM2Unit(29.7);
    guidoPageFormat.height = GuidoCM2Unit(29.7/ratio);
    guidoPageFormat.marginright = 0;

    graphicsScene->clear();
    guidoGraphicsItem = new QGuidoGraphicsItem();
    guidoGraphicsItem->setGMNCode(currentSong.gmn);
    guidoGraphicsItem->setGuidoPageFormat(guidoPageFormat);
    graphicsScene->addItem(guidoGraphicsItem);

    QList<Score> scores = GuidoService::gmnToScores(currentSong.gmn);
    MyMapCollector eventMapCollector;
    GuidoErrCode guidoErrCode = GuidoGetMap(guidoGraphicsItem->getGRHandler(), currentPage, 317, 317/ratio, kGuidoEvent, eventMapCollector);
    for (MapElement mapElement : eventMapCollector.mapElements) {
        if (mapElement.second.infos().type != kNote)
            continue;

        FloatRect floatRect = mapElement.first;
        QRectF rect;
        rect.setCoords(floatRect.left, floatRect.top, floatRect.right, floatRect.bottom);

        GraphicsScoreItem *item = new GraphicsScoreItem(rect);
        QObject::connect(item, &GraphicsScoreItem::gotNoteOnEvent, this, &GuidoView::playNoteOnEvent);
        QObject::connect(item, &GraphicsScoreItem::gotNoteOffEvent, this, &GuidoView::playNoteOffEvent);

        Score score = GuidoService::getScore(scores, mapElement);
        item->setScore(score);

        graphicsScene->addItem(item);
    }

    gv->fitInView(guidoGraphicsItem, Qt::KeepAspectRatio);
}

void GuidoView::nextPage() {
    currentPage = qMax(guidoGraphicsItem->pageCount(), currentPage+1);
    guidoGraphicsItem->setPage(currentPage);
}

void GuidoView::previousPage() {
    currentPage = qMin(1, currentPage-1);
    guidoGraphicsItem->setPage(currentPage);
}

void GuidoView::playNoteOnEvent(NoteOnEvent event) {
    emit gotNoteOnEvent(event);
}

void GuidoView::playNoteOffEvent(NoteOffEvent event) {
    emit gotNoteOffEvent(event);
}

void GuidoView::on_comboBox_currentIndexChanged(int index)
{
    currentSong = songs.at(index);
    currentPage = 1;
}
