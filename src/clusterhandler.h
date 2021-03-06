#ifndef CLUSTERHANDLER_H
#define CLUSTERHANDLER_H

#include <QList>
#include <QObject>

#include "events.h"

class QTimer;

class ClusterHandler : public QObject
{
    Q_OBJECT
public:
    ClusterHandler();

public slots:
    void matchNoteOnEvent(NoteOnEvent noteOn);
    void matchNoteOffEvent(NoteOffEvent noteOff);

signals:
    void gotControlChangeEvent(ControlChangeEvent event);
    void gotMidiPairCluster(MidiPairCluster midiPairCluster);
    void gotMidiPairClusters(QList<MidiPairCluster> midiPairClusters);
    void reset();

private:
    QTimer *delayTimer;
    QTimer *resetTimer;
    QList<ChannelEvent> channelEvents;
    QList<MidiPairCluster> midiPairClusters;
    bool pressedKeys[128];

    static constexpr int CHORD_DELAY = 50;
    static constexpr int RESET_DELAY = 2000;

private slots:
    void timeOutDelay();
    void timeOutReset();
};

#endif // CLUSTERHANDLER_H
