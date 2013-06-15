#ifndef GUIDOSERVICE_H
#define GUIDOSERVICE_H

#include <QString>
#include <QList>

#include "GUIDOScoreMap.h"

class Score;

class GuidoService
{
public:
    GuidoService();

    static QList<Score> gmnToScores5(const QString &gmn);
    static QList<Score> gmnToScores(const QString &gmn);
    static Score getScore(const QList<Score>& scores, const MapElement& mapElement);
};

#endif // GUIDOSERVICE_H
