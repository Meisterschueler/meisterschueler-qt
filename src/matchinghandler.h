#ifndef MATCHINGHANDLER_H
#define MATCHINGHANDLER_H

#include <QList>
#include <QObject>

#include "Fraction.h"

#include "events.h"
#include "matchingitem.h"

class MatchingHandler : public QObject
{
    Q_OBJECT
public:
    MatchingHandler(const QList<MatchingItem>& matchingItems);

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
    QList<NoteEventPair> midiEvents;
};

#endif // MATCHINGHANDLER_H
