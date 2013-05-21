#ifndef HANONSONGFACTORY_H
#define HANONSONGFACTORY_H

#include <QList>
#include <QString>
#include <QVector>

class Song;
class Score;

class Sequence {
public:
    Sequence(QString pattern, QVector<int> fingers, int steps[]) {
        m_pattern = pattern;
        m_fingers = fingers;
        m_steps = steps;
    }

    QString m_pattern;
    QVector<int> m_fingers;
    int * m_steps;
};

class Sequence2 : public Sequence{
public:
    Sequence2(QString pattern, QVector<int> leftFingers, QVector<int> rightFingers, int steps[]) : Sequence(pattern, leftFingers, steps){
        m_leftFingers = leftFingers;
        m_rightFingers = rightFingers;
    }

    QVector<int> m_leftFingers;
    QVector<int> m_rightFingers;
};

class HanonSongFactory
{
public:
    HanonSongFactory();

    QList<Song> getSongs();
    Song getNo(int no);
    QList<Score> sequencesToScores(QList<Sequence> sequences);
};



#endif // HANONSONGFACTORY_H
