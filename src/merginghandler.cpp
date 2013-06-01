#include "merginghandler.h"

#include <QList>

#include "needlemanwunsch.h"

MergingHandler::MergingHandler(QObject *parent) :
    QObject(parent)
{
}

void MergingHandler::eatMatchingItem(MatchingItem matchingItem) {
    const int FAIL_DIFFERENCE = 2; // Depends on Needleman. 0 is MATCH, 1 is WRONG, >=2 is FAILED

    QList<Score> scores;

    QByteArray beforePattern;
    QList<Score> scoresBefore = this->matchingItem.mergedScores;
    for (Score score : scoresBefore) {
        if (score.status != OPEN) {
            if (scores.indexOf(score) == -1 && score.status != OPEN) {
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

    for (int i = 0; i < alignment.size(); ++i) {
        if (alignment.at(i) == NeedlemanWunsch::DELETED) {
            emit scoreDeleted(scores.at(i));
        } else if (alignment.at(i) == NeedlemanWunsch::INSERT) {
            emit scoreInserted(scores.at(i));
        }
    }

    this->matchingItem = matchingItem;
}
