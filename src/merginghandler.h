#ifndef MERGINGHANDLER_H
#define MERGINGHANDLER_H

#include <QObject>

#include "Fraction.h"

#include "matchingitem.h"

class MergingHandler : public QObject
{
    Q_OBJECT
public:
    explicit MergingHandler(QObject *parent = 0);
    
signals:
    void reset();
    void scoreChanged(int idx, Score before, Score after);
    void positionChanged(Fraction position);
    
public slots:
    void eatMatchingItem(MatchingItem matchingItem);
};

#endif // MERGINGHANDLER_H
