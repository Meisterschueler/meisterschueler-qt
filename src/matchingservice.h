#ifndef MATCHINGSERVICE_H
#define MATCHINGSERVICE_H

#include <QList>
#include <QString>

class NoteEventPair;

class MatchingService
{
public:
    MatchingService();

    static QString midiEvents2pitchSequence(QList<NoteEventPair> events);
    static QString midiEvents2intervalSequence(QList<NoteEventPair> events);
    static QString midiEvents2pressedSequence(QList<NoteEventPair> events);
};

#endif // MATCHINGSERVICE_H
