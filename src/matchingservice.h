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

    static QByteArray getAlingment(QByteArray scorePitchSequence, QByteArray midiPitchSequence, QByteArray oldAlignment = "");
    static char getTransposition(QByteArray scorePitchSequence, QByteArray midiPitchSequence, QByteArray intervalAlignment);
    static double getQuality(QByteArray pitchAlignment, char transposition);
    static bool isFinished(QByteArray pitchAlignment, QByteArray pressedSequence);
    static QList<NoteEventPair> cutMatchingMidiEvents(QList<NoteEventPair> &events, QByteArray pitchAlignment);
};

#endif // MATCHINGSERVICE_H
