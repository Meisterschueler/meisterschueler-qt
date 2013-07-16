#include "merginghandler.h"

#include <QList>

#include "needlemanwunsch.h"

MergingHandler::MergingHandler(QObject *parent) :
    QObject(parent)
{
}

void MergingHandler::eatMatchingItem(MatchingItem matchingItem) {

    manageCID(matchingItem);
    managePositionChanged(matchingItem);

    this->matchingItem = matchingItem;
}

void MergingHandler::manageCID(MatchingItem matchingItem) {
    const int FAIL_DIFFERENCE = 2; // Depends on Needleman. 0 is MATCH, 1 is WRONG, >=2 is FAILED

    QList<Score> scores;

    QByteArray beforePattern;
    QList<Score> scoresBefore = this->matchingItem.mergedScores;
    for (Score score : scoresBefore) {
        if (score.status != OPEN) {
            if (scores.indexOf(score) == -1) {
                scores.append(score);
            }
            beforePattern.append(scores.indexOf(score)*FAIL_DIFFERENCE);
        }
    }

    QByteArray afterPattern;
    QList<Score> scoresAfter = matchingItem.mergedScores;
    for (Score score : scoresAfter) {
        if (score.status != OPEN) {
            if (scores.indexOf(score) == -1) {
                scores.append(score);
            }
            afterPattern.append(scores.indexOf(score)*FAIL_DIFFERENCE);
        }
    }

    NeedlemanWunsch needlemanWunsch;
    QByteArray alignment = needlemanWunsch.getAlignments(beforePattern, afterPattern);

    int idxBefore = 0;
    int idxAfter = 0;
    for (int i = 0; i < alignment.size(); ++i) {
        if (alignment.at(i) == NeedlemanWunsch::DELETED) {
            emit scoreDeleted(scores.at(i));
            idxBefore++;
        } else if (alignment.at(i) == NeedlemanWunsch::INSERT) {
            emit scoreInserted(scores.at(i));
            idxAfter++;
        } else if (alignment.at(i) == NeedlemanWunsch::MATCH) {
            if (scoresBefore.at(idxBefore) != scoresAfter.at(idxAfter)) {
                emit scoreChanged(scoresBefore.at(idxBefore), scoresAfter.at(idxAfter));
            }
            idxBefore++;
            idxAfter++;
        }
    }
}

void MergingHandler::managePositionChanged(MatchingItem matchingItem) {
    int oldIdx = this->matchingItem.pitchAlignment.lastIndexOf("m");
    int newIdx = matchingItem.pitchAlignment.lastIndexOf("m");

    Fraction oldPosition;
    if (oldIdx != -1) {
        oldPosition = this->matchingItem.mergedScores.at(oldIdx).position;
    }

    Fraction newPosition;
    if (newIdx != -1) {
        newPosition = matchingItem.mergedScores.at(newIdx).position;
    }

    if (oldPosition != newPosition) {
        emit positionChanged(newPosition);
    }
}
