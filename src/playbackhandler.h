#ifndef PLAYBACKHANDLER_H
#define PLAYBACKHANDLER_H

#include <QObject>
#include <QList>

#include "Fraction.h"

#include "events.h"

class QTimer;

class PlaybackHandler : public QObject
{
    Q_OBJECT
public:
    explicit PlaybackHandler(QObject *parent = 0);
    
    void setMidiPairs(QList<MidiPair> midiPairs);
    void setChannelEvents(QList<ChannelEvent> channelEvents);

private:
    int idx;
    QList<ChannelEvent> events;
    QTimer *timer;

signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);
    
public slots:
    void play();
    void stop();
    void jumpTo(Fraction position);

private slots:
    void playNextEvent();
};

#endif // PLAYBACKHANDLER_H
