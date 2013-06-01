#ifndef RESULTSERVICE_H
#define RESULTSERVICE_H

#include <QObject>

#include "matchingitem.h"

class ResultService : public QObject
{
    Q_OBJECT
public:
    explicit ResultService(QObject *parent = 0);
    
signals:
    
public slots:
    void analyseFinishedSong(MatchingItem item);
};

#endif // RESULTSERVICE_H
