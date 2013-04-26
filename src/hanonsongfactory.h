#ifndef HANONSONGFACTORY_H
#define HANONSONGFACTORY_H

#include <QList>

#include "song.h"
#include "score.h"

class Sequence {
public:
    Sequence(QString pattern, int fingers[], int steps[]) {
        m_pattern = pattern;
        m_fingers = fingers;
        m_steps = steps;
    }

    QString m_pattern;
    int * m_fingers;
    int * m_steps;
};

class Sequence2 : public Sequence{
public:
    Sequence2(QString pattern, int leftFingers[], int rightFingers[], int steps[]) : Sequence(pattern, leftFingers, steps){
        m_leftFingers = leftFingers;
        m_rightFingers = rightFingers;
    }

    int * m_leftFingers;
    int * m_rightFingers;
};

class HanonSongFactory
{
public:
    HanonSongFactory();

    Song getNo(int no);
    QList<Score> sequencesToScores(QList<Sequence> sequences);
};



#endif // HANONSONGFACTORY_H
