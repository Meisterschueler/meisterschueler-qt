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

private:
    MatchingItem matchingItem;
    
signals:
    void reset();
    void scoreChanged(Score before, Score after);
    void scoreInserted(Score score);
    void scoreDeleted(Score score);

    void positionChanged(Fraction position);

public slots:
    void eatMatchingItem(MatchingItem matchingItem);

private:
    void manageCID(MatchingItem matchingItem);
    void managePositionChanged(MatchingItem matchingItem);
};

#endif // MERGINGHANDLER_H
