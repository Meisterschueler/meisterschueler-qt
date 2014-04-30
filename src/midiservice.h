#ifndef MIDISERVICE_H
#define MIDISERVICE_H

#include <QFile>
#include <QList>

class MidiPair;
class MidiPairCluster;
class ChannelEvent;
class NoteOnEvent;
class NoteOffEvent;

class MidiService
{
public:
    MidiService();

    static int addNoteOn(QList<MidiPairCluster> &pairClusters, const NoteOnEvent& noteOn);
    static int addNoteOff(QList<MidiPairCluster> &pairClusters, const NoteOffEvent& noteOff);

    static bool save(const QString& fileName, const QList<ChannelEvent>& events);
    static QList<ChannelEvent> load(const QString fileName);
};

#endif // MIDISERVICE_H
