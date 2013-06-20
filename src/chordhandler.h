#ifndef CHORDHANDLER_H
#define CHORDHANDLER_H

#include <QMap>
#include <QObject>

#include "events.h"
#include "matchingitem.h"

class ChordHandler : public QObject
{
    Q_OBJECT
public:
    explicit ChordHandler(QObject *parent = 0);

private:
    QMap<QString, QString> chords;
    QMap<QString, QString> scales;

    QList<NoteOnEvent> noteOnEvents;
    
signals:
    void chordRecognized(MatchingItem matchingItem);
    
public slots:
    void reset();
    void matchNoteOnEvent(NoteOnEvent noteOn);
    void matchNoteOffEvent(NoteOffEvent noteOff);
    void matchChannelEvents(QList<ChannelEvent> channelEvents);

private:
    void match();
};

#endif // CHORDHANDLER_H
