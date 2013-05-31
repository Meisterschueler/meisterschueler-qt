#ifndef MIDISERVICE_H
#define MIDISERVICE_H

#include <QFile>
#include <QList>

class MidiPair;
class ChannelEvent;
class NoteOnEvent;
class NoteOffEvent;

class MidiService
{
public:
    MidiService();

    static void addNoteOn(QList<MidiPair> &pairs, const NoteOnEvent& noteOn);
    static void addNoteOff(QList<MidiPair> &pairs, const NoteOffEvent& noteOff);

    static void save(const QString& fileName, const QList<ChannelEvent>& events);
    static QList<ChannelEvent> load(const QString fileName);
};

#endif // MIDISERVICE_H
