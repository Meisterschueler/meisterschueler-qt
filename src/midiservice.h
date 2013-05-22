#ifndef MIDISERVICE_H
#define MIDISERVICE_H

#include <QFile>
#include <QList>

class NoteEventPair;
class NoteOnEvent;
class NoteOffEvent;

class MidiService
{
public:
    MidiService();

    static void addNoteOn(QList<NoteEventPair> &pairs, const NoteOnEvent& noteOn);
    static void addNoteOff(QList<NoteEventPair> &pairs, const NoteOffEvent& noteOff);

    static void save(const QString& fileName, const QList<NoteEventPair>& pairs);
    static QList<NoteEventPair> load(const QString fileName);
};

#endif // MIDISERVICE_H
