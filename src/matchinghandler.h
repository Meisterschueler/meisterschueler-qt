#ifndef MATCHINGHANDLER_H
#define MATCHINGHANDLER_H

#include <QList>
#include <QObject>

#include "Fraction.h"

#include "events.h"

class MatchingItem;

class MatchingHandler : public QObject
{
    Q_OBJECT
public:
    MatchingHandler();

public slots:
    void reset();
    void noteOnEvent(NoteOnEvent ev);
    void noteOffEvent(NoteOffEvent ev);

signals:
    void positionChanged(Fraction position);
    void songRecognized(MatchingItem item);
    void songFinished(MatchingItem item);

private:
    QList<MatchingItem> matchingItems;
    QList<MidiPair> midiEvents;
};

#endif // MATCHINGHANDLER_H
