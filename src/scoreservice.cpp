#include "scoreservice.h"

#include <QString>

#include "score.h"

ScoreService::ScoreService()
{
}

QList<Score> ScoreService::transposeStep(const QList<Score>& scores, const int& step) {
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

QList<Score> ScoreService::concat(const QList<Score>& scores1, const QList<Score>& scores2) {
    QList<Score> result;
    Fraction offset = scores1.last().position + scores1.last().duration;
    result.append(scores1);
    for (Score score : scores2) {
        score.position += offset;
        result.append(score);
    }
    return result;
}

QList<Score> ScoreService::merge(const QList<Score>& scores1, const QList<Score>& scores2) {
    QList<Score> result;
    result.append(scores1);
    result.append(scores2);
    qSort(result);
    return result;
}
