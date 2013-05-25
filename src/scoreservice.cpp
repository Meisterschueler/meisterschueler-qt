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

QList<Score> ScoreService::transposeSteps(const QList<Score>& scores, const QVector<int>& steps) {
    QList<Score> result;
    for (int step : steps) {
        result = concat(result, transposeStep(scores, step));
    }
    return result;
}

QList<Score> ScoreService::addFingers(const QList<Score>& scores, const QVector<int>& fingers) {
    QList<Score> result(scores);
    for (int i=0; i < result.size(); i++) {
        int idx = i % fingers.size();
        switch (fingers[idx]) {
        case 5:
            result[i].finger = LITTLE;
            break;
        case 4:
            result[i].finger = RING;
            break;
        case 3:
            result[i].finger = MIDDLE;
            break;
        case 2:
            result[i].finger = POINTER;
            break;
        case 1:
            result[i].finger = THUMB;
            break;
        }
    }
    return result;
}

QList<Score> ScoreService::concat(const QList<Score>& scores1, const QList<Score>& scores2) {
    QList<Score> result;
    Fraction offset(0);
    if (!scores1.isEmpty()) {
        offset = scores1.last().position + scores1.last().duration;
    }

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

QByteArray ScoreService::scoresToPitchSequence(const QList<Score>& scores) {
    QByteArray result;

    for (Score score : scores) {
        result.append(score.pitch);
    }

    return result;
}

QByteArray ScoreService::scoresToIntervalSequence(const QList<Score>& scores) {
    QByteArray result;

    QByteArray pitchSequence = scoresToPitchSequence(scores);
    for (int i = 1; i < pitchSequence.length(); i++ ) {
        result.append(pitchSequence.at(i) - pitchSequence.at(i-1));
    }

    return result;
}

QList<Score> ScoreService::filterFingers(const QList<Score>& scores, const int& fingers) {
    QList<Score> result;
    for (Score score : scores) {
        if (score.finger & fingers) {
            result.append(score);
        }
    }
    return result;
}

QList<Score> ScoreService::filterStatus(const QList<Score>& scores, const int& status) {
    QList<Score> result;
    for (Score score : scores) {
        if (score.status & status) {
            result.append(score);
        }
    }
    return result;
}
