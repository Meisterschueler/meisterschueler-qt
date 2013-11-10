#ifndef CUSTOMVIEW_H
#define CUSTOMVIEW_H

#include <QList>

#include "qcustomplot.h"

#include "events.h"

class CustomView : public QCustomPlot
{
    Q_OBJECT
public:
    explicit CustomView(QWidget *parent = 0);
    
signals:
    
public slots:
    void showMidiPairs(QList<MidiPair> midiPairs);
    
private:
    QList<MidiPair> midiPairs;
};

#endif // CUSTOMVIEW_H
