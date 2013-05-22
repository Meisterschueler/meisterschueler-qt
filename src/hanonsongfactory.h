#ifndef HANONSONGFACTORY_H
#define HANONSONGFACTORY_H

#include <QList>
#include <QString>
#include <QVector>

class Song;
class Score;

class Sequence {
public:
    Sequence(const QString& pattern, const QVector<int>& fingers, const QVector<int>& steps) {
        m_pattern = pattern;
        m_fingers = fingers;
        m_steps = steps;
    }

    QString m_pattern;
    QVector<int> m_fingers;
    QVector<int> m_steps;
};

class Sequence2 : public Sequence{
public:
    Sequence2(const QString& pattern, const QVector<int>& leftFingers, const QVector<int>& rightFingers, const QVector<int>& steps) : Sequence(pattern, leftFingers, steps){
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

    QList<Song> getSongs() const;
    Song getNo(int no) const;
    QList<Score> sequencesToScores(const QList<Sequence>& sequences) const;
};



#endif // HANONSONGFACTORY_H
