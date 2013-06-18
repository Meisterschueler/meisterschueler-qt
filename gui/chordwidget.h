#ifndef CHORDWIDGET_H
#define CHORDWIDGET_H

#include "QGuidoWidget.h"

#include "matchingitem.h"

class ChordWidget : public QGuidoWidget
{
    Q_OBJECT
public:
    explicit ChordWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    void showChord(MatchingItem matchingItem);
    
};

#endif // CHORDWIDGET_H
