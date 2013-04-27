#include "scoreservice.h"

#include <QString>

#include "score.h"

ScoreService::ScoreService()
{
}

QList<Score> ScoreService::transposeStep(QList<Score> scores, int step) {
    const QString whiteKeys = "x.x.xx.x.x.xx.x.xx.x.x.xx.x.xx.x.x.xx.x.xx.x.x.xx.x.xx.x.x.xx.x.xx.x.x.xx.x.xx.x.x.xx.x.xx.x.x.xx.x.xx.x.x.xx.x.xx.x.x.xx.x.xx.x";

    QList<Score> result;

    for (Score score : scores) {
        int oldPitch = score.pitch;
        int newPitch = oldPitch;
        if (step > 0) {
            for (int i = 0; i < step; i++) {
                newPitch = whiteKeys.indexOf("x", newPitch+1);
            }
        } else if (step < 0) {
            for (int i = 0; i < -step; i++) {
                newPitch = whiteKeys.lastIndexOf("x", newPitch-1);
            }
        }
        Score transposed(score);
        transposed.pitch = newPitch;
        result.append(transposed);
    }
    return result;
}

QList<Score> ScoreService::transposeSteps(QList<Score> scores, int * steps) {
    QList<Score> result;
    return result;
}

QList<Score> ScoreService::addFingers(QList<Score> scores, int * fingers) {
    QList<Score> result;
    return result;
}

QList<Score> ScoreService::concat(QList<Score> scores1, QList<Score> scores2) {
    QList<Score> result;
    return result;
}

QList<Score> ScoreService::merge(QList<Score> scores1, QList<Score> scores2) {
    QList<Score> result;
    return result;
}
