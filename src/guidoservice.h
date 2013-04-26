#ifndef GUIDOSERVICE_H
#define GUIDOSERVICE_H

#include <QString>
#include <QList>

#include "events.h"
#include "score.h"

class GuidoService
{
public:
    GuidoService();

    static QList<Score> gmnToScores(const QString &gmn);
};

#endif // GUIDOSERVICE_H
