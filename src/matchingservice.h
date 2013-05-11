#ifndef MATCHINGSERVICE_H
#define MATCHINGSERVICE_H

#include <QByteArray>
#include <QList>

class NoteEventPair;

class MatchingService
{
public:
    MatchingService();

    static const char PRESSED = 'X';
    static const char RELEASED = '.';

    static QByteArray midiEvents2pitchSequence(QList<NoteEventPair> events);
    static QByteArray midiEvents2intervalSequence(QList<NoteEventPair> events);
    static QByteArray midiEvents2pressedSequence(QList<NoteEventPair> events);

    static char getTransposition(QByteArray midiPitchSequence, QByteArray scorePitchSequence, QByteArray intervalAlignment);
};

#endif // MATCHINGSERVICE_H
