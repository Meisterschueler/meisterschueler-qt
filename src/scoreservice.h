#ifndef SCORESERVICE_H
#define SCORESERVICE_H

#include <QList>
#include <QString>

#include "score.h"

class ScoreService
{
public:
    ScoreService();

    //const QString whiteKeys = QString("x.x.xx.x.x.x").repeated(11).left(128);

    static QList<Score> transposeStep(QList<Score> scores, int step);
    static QList<Score> transposeSteps(QList<Score> scores, int * steps);
    static QList<Score> addFingers(QList<Score> scores, int * fingers);
    static QList<Score> concat(QList<Score> scores1, QList<Score> scores2);
    static QList<Score> merge(QList<Score> scores1, QList<Score> scores2);
};

#endif // SCORESERVICE_H
