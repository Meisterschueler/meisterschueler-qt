#include "chordwidget.h"

#include <QString>

#include "matchingitem.h"

ChordWidget::ChordWidget(QWidget *parent) :
    QGuidoWidget(parent)
{
    setGMNCode("[_]");
}

void ChordWidget::showChord(MatchingItem matchingItem) {
    QString gmn = matchingItem.song.gmn;
    setGMNCode(gmn);
}
