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
    for (Score score : this->matchingItem.mergedScores) {
        if (scores.indexOf(score) == -1 && score.status != OPEN) {
            scores.append(score);
        }
    }

    for (Score score : matchingItem.mergedScores) {
        if (scores.indexOf(score) == -1 && score.status != OPEN) {
            scores.append(score);
        }
    }

    QByteArray before;
    for (Score score : this->matchingItem.mergedScores) {
        if (score.status != OPEN) {
            before.append(scores.indexOf(score)*FAIL_DIFFERENCE);
        }
    }

    QByteArray after;
    for (Score score : matchingItem.mergedScores) {
        if (score.status != OPEN) {
            after.append(scores.indexOf(score)*FAIL_DIFFERENCE);
        }
    }

    NeedlemanWunsch needlemanWunsch;
    QByteArray alignment = needlemanWunsch.getAlignments(before, after);

    for (int i = 0; i < alignment.size(); ++i) {
        if (alignment.at(i) == NeedlemanWunsch::DELETED) {
            emit scoreDeleted(scores.at(i));
        } else if (alignment.at(i) == NeedlemanWunsch::INSERT) {
            emit scoreInserted(scores.at(i));
        }
    }

    this->matchingItem = matchingItem;
}
