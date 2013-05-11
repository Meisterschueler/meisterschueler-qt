#ifndef MATCHINGHANDLER_H
#define MATCHINGHANDLER_H

#include <QSharedPointer>
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
    void noteOnEvent(NoteOnEvent noteOn);
    void noteOffEvent(NoteOffEvent noteOff);

signals:
    void positionChanged(Fraction position);
    void songRecognized(MatchingItem item);
    void songFinished(MatchingItem item);

private:
    QList<MatchingItem> matchingItems;
    QSharedPointer<QList<NoteEventPair>> midiEvents;

    void match();
};

#endif // MATCHINGHANDLER_H
