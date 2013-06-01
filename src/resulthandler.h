#ifndef RESULTHANDLER_H
#define RESULTHANDLER_H

#include <QObject>

#include "matchingitem.h"

class ResultHandler : public QObject
{
    Q_OBJECT
public:
    explicit ResultHandler(QObject *parent = 0);
    
signals:
    
public slots:
    void analyseFinishedSong(MatchingItem item);
};

#endif // RESULTHANDLER_H
