#ifndef MATCHINGSERVICE_H
#define MATCHINGSERVICE_H

#include <QList>
#include <QString>

#include "events.h"

class MatchingService
{
public:
    MatchingService();

    static QString midiEvents2pitchSequence(QList<MidiPair> events);
    static QString midiEvents2intervalSequence(QList<MidiPair> events);
    static QString midiEvents2pressedSequence(QList<MidiPair> events);
};

#endif // MATCHINGSERVICE_H
