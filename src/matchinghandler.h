#ifndef MATCHINGHANDLER_H
#define MATCHINGHANDLER_H

#include <QSharedPointer>
#include <QList>
#include <QObject>

#include "events.h"
#include "matchingitem.h"

class MatchingHandler : public QObject
{
    Q_OBJECT
public:
    MatchingHandler(const QList<MatchingItem>& matchingItems);

signals:
    void songRecognized(MatchingItem item);
    void songFinished(MatchingItem item);

public slots:
    void init();
    void reset();
    void matchNoteOnEvent(NoteOnEvent noteOn);
    void matchNoteOffEvent(NoteOffEvent noteOff);
    void matchChannelEvents(QList<ChannelEvent> channelEvents);
    void matchChannelEvents2(QList<ChannelEvent> channelEvents);

private:
    QList<MatchingItem> matchingItems;
    QSharedPointer<QList<MidiPairCluster>> midiPairClusters;
    QSharedPointer<QByteArray> oldPitchSequence;

    void match();
    void disableBadItems(const double& lowerQualityLimit);
    void prepareAndEmitFinishedItem(const MatchingItem &item);

};

#endif // MATCHINGHANDLER_H
