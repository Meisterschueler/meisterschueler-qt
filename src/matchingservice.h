#ifndef MATCHINGSERVICE_H
#define MATCHINGSERVICE_H

#include <QByteArray>
#include <QList>
#include <QString>

class NoteEventPair;

class MatchingService
{
public:
    MatchingService();

    static const char PRESSED = 'X';
    static const char RELEASED = '.';

    static QString midiEvents2pitchSequence(QList<NoteEventPair> events);
    static QByteArray midiEvents2intervalSequence(QList<NoteEventPair> events);
    static QString midiEvents2pressedSequence(QList<NoteEventPair> events);
};

#endif // MATCHINGSERVICE_H
