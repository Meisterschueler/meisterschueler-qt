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

    static QByteArray midiEvents2pitchSequence(const QList<NoteEventPair>& events);
    static QByteArray midiEvents2intervalSequence(const QList<NoteEventPair>& events);
    static QByteArray midiEvents2pressedSequence(const QList<NoteEventPair>& events);

    static QByteArray getAlingment(const QByteArray& scorePitchSequence, const QByteArray& midiPitchSequence, const char& transposition=0, const QByteArray& oldAlignment = "");
    static QByteArray getSaveAlignment(const QByteArray& alignment);
    static char getTransposition(const QByteArray& scorePitchSequence, const QByteArray& midiPitchSequence, const QByteArray& intervalAlignment);
    static double getQuality(const QByteArray& pitchAlignment, char transposition);
    static bool isFinished(const QByteArray& pitchAlignment, const QByteArray& pressedSequence);
    static QList<NoteEventPair> cutMatchingMidiEvents(QList<NoteEventPair> &events, const QByteArray& pitchAlignment);
};

#endif // MATCHINGSERVICE_H
