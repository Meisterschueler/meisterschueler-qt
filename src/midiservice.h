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

    static void addNoteOn(QList<NoteEventPair> &pairs, NoteOnEvent noteOn);
    static void addNoteOff(QList<NoteEventPair> &pairs, NoteOffEvent noteOff);

    static void save(const QFile *file, const QList<NoteEventPair>& pairs);
    static QList<NoteEventPair> load(const QFile *file);
};

#endif // MIDISERVICE_H
