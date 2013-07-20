#ifndef RESULTHANDLER_H
#define RESULTHANDLER_H

#include <QObject>

#include "matchingitem.h"

class ResultManager : public QObject
{
    Q_OBJECT
public:
    explicit ResultManager(QObject *parent = 0);
    
signals:
    
public slots:
    void analyseFinishedSong(MatchingItem item);
    void saveFinishedSong(MatchingItem item);
};

#endif // RESULTHANDLER_H
