#ifndef SCORESERVICE_H
#define SCORESERVICE_H

#include <QByteArray>
#include <QList>
#include <QString>
#include <QVector>

class Score;

class ScoreService
{
public:
    ScoreService();

    //const QString whiteKeys = QString("x.x.xx.x.x.x").repeated(11).left(128);

    static QList<Score> transposeStep(const QList<Score> &scores, const int &step);
    static QList<Score> transposeSteps(QList<Score> scores, int * steps);
    static QList<Score> addFingers(const QList<Score> &scores, const QVector<int>& fingers);
    static QList<Score> concat(const QList<Score> &scores1, const QList<Score> &scores2);
    static QList<Score> merge(const QList<Score> &scores1, const QList<Score> &scores2);

    static QByteArray scoresToPitchSequence(const QList<Score> scores);
    static QByteArray scoresToIntervalSequence(const QList<Score> scores);
};

#endif // SCORESERVICE_H
