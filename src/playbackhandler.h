#ifndef PLAYBACKHANDLER_H
#define PLAYBACKHANDLER_H

#include <QObject>

#include "events.h"

class PlaybackHandler : public QObject
{
    Q_OBJECT
public:
    explicit PlaybackHandler(QObject *parent = 0);
    
signals:
    void gotNoteOnEvent(NoteOnEvent event);
    void gotNoteOffEvent(NoteOffEvent event);
    
public slots:
    void playMidiPairs(QList<MidiPair> midiPairs);
};

#endif // PLAYBACKHANDLER_H
