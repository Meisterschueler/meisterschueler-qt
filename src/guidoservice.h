#ifndef GUIDOSERVICE_H
#define GUIDOSERVICE_H

#include <QString>
#include <QList>

class Score;

class GuidoService
{
public:
    GuidoService();

    static QList<Score> gmnToScores5(const QString &gmn);
    static QList<Score> gmnToScores(const QString &gmn);
};

#endif // GUIDOSERVICE_H
