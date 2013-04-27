#ifndef GUIDOSERVICE_H
#define GUIDOSERVICE_H

#include <QString>
#include <QList>

#include "events.h"

class Score;

class GuidoService
{
public:
    GuidoService();

    static QList<Score> gmnToScores(const QString &gmn);
};

#endif // GUIDOSERVICE_H
